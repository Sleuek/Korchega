/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#include "uavcan.hpp"
#include <kocherga/kocherga_uavcan.hpp>
#include <canard_stm32.h>
#include <board/board.hpp>
#include <hal.h>
#include <cstdlib>


namespace uavcan
{
namespace
{
/**
 * An adapter class that bridges the Libcanard STM32 driver with the UAVCAN bootloader interface driver.
 * Note that the logic of the return codes implemented in the Libcanard STM32 drivers matches the expectations
 * of the bootloader, so on this part no adaptation is needed.
 */
class UAVCANPlatform : public kocherga_uavcan::IUAVCANPlatform
{
    /**
     * Hardware FIFO is 3 frames deep; the shortest (worst case) frame duration with acceptance filters allowing only
     * extended frames is:
     *  (1+11+1+1+18+1+2+4+0+15+1+1+1+7) = 64 bits = 64 microseconds @ 1 Mbps
     *
     * Multiply by the depth of the FIFO:
     *  64 * 3 = 192 microseconds
     *
     * Therefore, we must read the driver not less frequently than every 192 microseconds, otherwise we might be
     * losing frames due to RX overrun. Therefore we enforce that the system tick interval is less than that.
     */
    static_assert((1000000 / CH_CFG_ST_FREQUENCY) < 180,
                  "Minimal delay must be lower in order for the libcanard STM32 driver to work properly");

    static constexpr std::uint8_t LEDUpdateIntervalMilliseconds = 25;

    bool had_activity_ = false;
    ::systime_t last_led_update_timestamp_st_ = 0;

    inline void wait()
    {
        if (chVTTimeElapsedSinceX(last_led_update_timestamp_st_) >= TIME_MS2I(LEDUpdateIntervalMilliseconds))
        {
            last_led_update_timestamp_st_ =
                ::systime_t(last_led_update_timestamp_st_ + TIME_MS2I(LEDUpdateIntervalMilliseconds));

            board::setCANActivityLED(0, had_activity_);
            had_activity_ = false;
        }

        chThdSleep(1);      // Sleeping for a minimal possible time interval
    }

    void resetWatchdog() override
    {
        // Nothing to do
    }

    void sleep(std::chrono::microseconds duration) const override
    {
        chThdSleepMicroseconds(duration.count());
    }

    std::uint64_t getRandomUnsignedInteger(std::uint64_t lower_bound,
                                           std::uint64_t upper_bound) const override
    {
        const auto rnd = std::uint64_t(std::rand()) * std::uint64_t(std::rand());
        return lower_bound + rnd % (upper_bound - lower_bound);
    }

    std::int16_t configure(std::uint32_t bitrate,
                           CANMode mode,
                           const CANAcceptanceFilterConfig& acceptance_filter) override
    {
        DEBUG_LOG("CAN init: %9u bps, mode %d, filt id 0x%x mask 0x%x\n",
                  unsigned(bitrate),
                  int(mode),
                  unsigned(acceptance_filter.id),
                  unsigned(acceptance_filter.mask));

        had_activity_ = false;
        last_led_update_timestamp_st_ = chVTGetSystemTimeX();

        // Computing CAN timings
        CanardSTM32CANTimings timings{};
        std::int16_t res = canardSTM32ComputeCANTimings(STM32_PCLK1, bitrate, &timings);
        if (res < 0)
        {
            return res;
        }

        // Initializing the interface
        CanardSTM32IfaceMode mode2{};
        switch (mode)
        {
        case CANMode::Normal:
        {
            mode2 = CanardSTM32IfaceModeNormal;
            break;
        }
        case CANMode::Silent:
        {
            mode2 = CanardSTM32IfaceModeSilent;
            break;
        }
        case CANMode::AutomaticTxAbortOnError:
        {
            mode2 = CanardSTM32IfaceModeAutomaticTxAbortOnError;
            break;
        }
        }

        res = canardSTM32Init(&timings, mode2);
        if (res < 0)
        {
            return res;
        }

        // Configuring acceptance filters
        CanardSTM32AcceptanceFilterConfiguration acceptance_filter_config;
        acceptance_filter_config.id   = acceptance_filter.id;
        acceptance_filter_config.mask = acceptance_filter.mask;

        res = canardSTM32ConfigureAcceptanceFilters(&acceptance_filter_config, 1);
        if (res < 0)
        {
            return res;
        }

        return 0;
    }

    std::int16_t send(const ::CanardCANFrame& frame, std::chrono::microseconds timeout) override
    {
        const auto started_at = chVTGetSystemTimeX();
        do
        {
            std::int16_t res = canardSTM32Transmit(&frame);      // Try to transmit
            if (res != 0)
            {
                had_activity_ |= res > 0;
                return res;                             // Either success or error, return
            }
            wait();                                     // No space in the buffer, skip the time quantum and try again
        }
        while (TIME_I2US(chVTTimeElapsedSinceX(started_at)) < timeout.count());

        return 0;                                       // Timed out
    }

    std::pair<std::int16_t, ::CanardCANFrame> receive(std::chrono::microseconds timeout) override
    {
        const auto started_at = chVTGetSystemTimeX();
        CanardCANFrame f{};
        do
        {
            std::int16_t res = canardSTM32Receive(&f);
            if (res != 0)
            {
                had_activity_ |= res > 0;
                return {res, f};                        // Either success or error, return
            }
            wait();                                     // Buffer is empty, skip the time quantum and try again
        }
        while (TIME_I2US(chVTTimeElapsedSinceX(started_at)) < timeout.count());

        return {0, f};                                  // Timed out
    }

    bool shouldExit() const override
    {
        return os::isShutdownRequested();
    }

    bool tryScheduleReboot() override
    {
        os::requestShutdown();
        return true;
    }

    UAVCANPlatform(const UAVCANPlatform&) = delete;
    UAVCANPlatform& operator=(const UAVCANPlatform&) = delete;

public:
    UAVCANPlatform() = default;
};


using BootloaderNode = kocherga_uavcan::BootloaderNode<1024 * 16>;


class Thread final : public chibios_rt::BaseStaticThread<8192>
{
    BootloaderNode& node_;
    const std::uint32_t can_bit_rate_;
    const std::uint8_t local_node_id_;
    const std::uint8_t file_server_node_id_;
    const senoval::String<200> remote_image_file_path_;

    void main() final
    {
        node_.run(can_bit_rate_,
                  local_node_id_,
                  file_server_node_id_,
                  remote_image_file_path_.c_str());

        assert(os::isShutdownRequested());
        std::puts("UAVCAN node down");
        os::requestShutdown();
    }

public:
    virtual ~Thread() = default;

    Thread(BootloaderNode& node,
           const std::uint32_t can_bit_rate,
           const std::uint8_t local_node_id,
           const std::uint8_t file_server_node_id,
           const char* const remote_image_file_path) :
        node_(node),
        can_bit_rate_(can_bit_rate),
        local_node_id_(local_node_id),
        file_server_node_id_(file_server_node_id),
        remote_image_file_path_(remote_image_file_path)
    { }
};

}  // namespace

static std::optional<BootloaderNode> g_node;


void init(kocherga::BootloaderController& bl,
          const std::uint32_t can_bit_rate,
          const std::uint8_t local_node_id,
          const std::uint8_t file_server_node_id,
          const char* const remote_image_file_path)
{
    kocherga_uavcan::HardwareInfo hw;

    // Hardware unique ID
    {
        const auto uid = board::readUniqueID();
        std::copy(uid.begin(),
                  uid.end(),
                  hw.unique_id.begin());
    }

    // Hardware version
    {
        const auto hw_ver = board::detectHardwareVersion();
        hw.major = hw_ver.major;
        hw.minor = hw_ver.minor;
    }

    // Device signature, if present
    if (auto sign = board::tryReadDeviceSignature())
    {
        hw.certificate_of_authenticity.resize(sign->size());
        std::copy(sign->begin(), sign->end(), hw.certificate_of_authenticity.begin());
    }

    // Construct the node
    static UAVCANPlatform platform;
    g_node.emplace(bl, platform, PRODUCT_ID_STRING, hw);

    // Launch the thread
    static Thread node_thread(*g_node,
                              can_bit_rate,
                              local_node_id,
                              file_server_node_id,
                              remote_image_file_path);
    node_thread.setName("uavcan");
    (void) node_thread.start(HIGHPRIO);  // High priority is required to avoid CAN frame loss - we don't have buffers
}


Parameters getParameters()
{
    Parameters p;
    if (g_node)
    {
        p.can_bus_bit_rate = g_node->getCANBusBitRate();
        p.local_node_id    = g_node->getLocalNodeID();
    }
    return p;
}

}
