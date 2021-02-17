/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#include "serial.hpp"
#include <kocherga/kocherga_popcop.hpp>
#include <board/board.hpp>
#include <board/usb/usb.hpp>
#include <hal.h>
#include <cstdlib>


namespace serial
{
namespace
{
/**
 * Platform abstraction class for the Popcop endpoint.
 */
class PopcopPlatform final : public kocherga_popcop::IPopcopPlatform
{
    static bool shouldUseUSB()
    {
        return board::usb::getState() == board::usb::State::Connected;
    }

    static ::sysinterval_t getIOTimeout()
    {
        const std::int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(IOByteTimeout).count();
        return TIME_MS2I(std::max<std::int64_t>(1, ms));
    }

    void resetWatchdog() override
    {
        // Nothing to do
    }

    void emit(std::uint8_t byte) override
    {
        if (shouldUseUSB())
        {
            (void) chnPutTimeout(board::usb::getSerialUSBDriver(), byte, getIOTimeout());
        }
        else
        {
            (void) chnPutTimeout(&STDIN_SD, byte, getIOTimeout());
        }
    }

    std::optional<std::uint8_t> receive() override
    {
        std::int32_t out = 0;

        if (shouldUseUSB())
        {
            out = chnGetTimeout(board::usb::getSerialUSBDriver(), getIOTimeout());
        }
        else
        {
            out = chnGetTimeout(&STDIN_SD, getIOTimeout());
        }

        if (out >= 0)
        {
            return std::uint8_t(out);
        }
        else
        {
            return {};
        }
    }

    void processUnhandledFrame(const popcop::transport::ParserOutput::Frame&) override
    {
        // Nothing to do
    }

    void processExtraneousData(const std::uint8_t*, std::size_t) override
    {
        // Nothing to do
    }

    popcop::standard::DeviceManagementCommandResponseMessage::Status processUnhandledDeviceManagementCommand(
        const popcop::standard::DeviceManagementCommandRequestMessage& request) override
    {
        if (request.command == popcop::standard::DeviceManagementCommand::Restart)
        {
            os::requestShutdown();
            return popcop::standard::DeviceManagementCommandResponseMessage::Status::Ok;
        }
        else
        {
            return popcop::standard::DeviceManagementCommandResponseMessage::Status::BadCommand;
        }
    }

    std::int16_t writeAndReadBackCertificateOfAuthenticity(const std::uint8_t* in_data,
                                                                 std::uint8_t* out_data,
                                                                 std::uint8_t length) override
    {
        if (length > 0)
        {
            DEBUG_LOG("CoA write %u bytes\n", length);

            board::DeviceSignature signature;
            std::copy_n(in_data,
                        std::min<std::size_t>(signature.max_size(), length),
                        signature.begin());

            if (!board::tryWriteDeviceSignature(signature))
            {
                DEBUG_LOG("CoA write failed\n");
                return -1;
            }
        }

        if (auto signature = board::tryReadDeviceSignature())
        {
            std::copy(signature->begin(), signature->end(), out_data);
            return std::int16_t(signature->size());
        }
        else
        {
            DEBUG_LOG("CoA read failed\n");
            return 0;
        }
    }

    bool shouldExit() const override
    {
        return os::isShutdownRequested();
    }

    PopcopPlatform(const PopcopPlatform&) = delete;
    PopcopPlatform& operator=(const PopcopPlatform&) = delete;

public:
    PopcopPlatform() = default;
};


class Thread final : public chibios_rt::BaseStaticThread<8192>
{
    kocherga_popcop::PopcopProtocol& endpoint_;

    void main() final
    {
        endpoint_.run();

        assert(os::isShutdownRequested());
        std::puts("Serial endpoint down");
        os::requestShutdown();
    }

public:
    virtual ~Thread() = default;

    explicit Thread(kocherga_popcop::PopcopProtocol& endpoint) :
        endpoint_(endpoint)
    { }
};

}  // namespace


void init(kocherga::BootloaderController& bl)
{
    // Observe that we do NOT initialize the software version info; it is picked up from the image (if present)!
    popcop::standard::EndpointInfoMessage info;

    info.endpoint_name        = PRODUCT_ID_STRING;
    info.endpoint_description = PRODUCT_NAME_STRING;
    // Environment info is not provided, we have to conserve ROM

    // Hardware unique ID
    {
        const auto uid = board::readUniqueID();
        std::copy(uid.begin(),
                  uid.end(),
                  info.globally_unique_id.begin());
    }

    // Hardware version
    {
        const auto hw_ver = board::detectHardwareVersion();
        info.hardware_version.major = hw_ver.major;
        info.hardware_version.minor = hw_ver.minor;
    }

    // Device signature, if present
    if (auto sign = board::tryReadDeviceSignature())
    {
        info.certificate_of_authenticity.resize(sign->size());
        std::copy(sign->begin(), sign->end(), info.certificate_of_authenticity.begin());
    }

    // Construct the node
    static PopcopPlatform platform;
    static kocherga_popcop::PopcopProtocol endpoint(bl, platform, info);

    // Launch the thread
    static Thread node_thread(endpoint);
    node_thread.setName("serial");
    (void) node_thread.start(NORMALPRIO);
}

}
