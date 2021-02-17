/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#include <ch.hpp>
#include <hal.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <os.hpp>

#include <kocherga/kocherga.hpp>

#include "board/board.hpp"
#include "board/usb/usb.hpp"
#include "uavcan/uavcan.hpp"
#include "serial/serial.hpp"
#include "app_shared/app_shared.hpp"
#include "rom_backend.hpp"


namespace app
{
namespace
{

constexpr std::chrono::seconds WatchdogTimeout(10);

constexpr std::chrono::seconds BootDelayAfterWatchdogTimedOut(10);


class Platform : public kocherga::IPlatform
{
    chibios_rt::Mutex mutex_;

    void lockMutex() override
    {
        mutex_.lock();
    }

    void unlockMutex() override
    {
        mutex_.unlock();
    }

    std::chrono::microseconds getMonotonicUptime() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(board::Clock::now().time_since_epoch());
    }
};


void setStatusLEDFromBootloaderState(const kocherga::State s)
{
    using kocherga::State;

    switch (s)
    {
    case State::NoAppToBoot:
    {
        board::setRGBLED(1.0F, 1.0F, 0.0F);     // YELLOW
        break;
    }
    case State::AppUpgradeInProgress:
    {
        board::setRGBLED(0.0F, 0.0F, 1.0F);     // BLUE
        break;
    }
    case State::BootCancelled:
    {
        board::setRGBLED(0.0F, 1.0F, 0.0F);     // GREEN
        break;
    }
    case State::BootDelay:                      // Equivalent to ReadyToBoot because the delay is zero
    case State::ReadyToBoot:
    {
        board::setRGBLED(1.0F, 0.0F, 1.0F);     // MAGENTA
        break;
    }
    default:
    {
        assert(false);
        board::setRGBLED(0.0F, 0.0F, 0.0F);
        break;
    }
    }
}


[[noreturn]]
void doBoot()
{
    assert(os::isShutdownRequested());          // Making sure the imminent shutdown was announced properly

    setStatusLEDFromBootloaderState(kocherga::State::ReadyToBoot);

    // Writing down the CAN bus parameters for the application, if known
    {
        const uavcan::Parameters p = uavcan::getParameters();

        app_shared::AppShared as;
        as.can_bus_speed  = p.can_bus_bit_rate;
        as.uavcan_node_id = p.local_node_id;
        assert((as.can_bus_speed  <= 1000000) &&
               (as.uavcan_node_id <= 127));

        app_shared::writeSharedStruct(as);
    }

    board::kickWatchdog();    // The final reset, the application will have time to boot and init until next timeout
    board::bootApplication();
}

}  // namespace
}  // namespace app


int main()
{
    const auto reset_cause = board::init(app::WatchdogTimeout);

    board::setRGBLED(1.0F, 1.0F, 1.0F);                 // Initial state
    DEBUG_LOG("\n>>> DEBUG <<<\n\n");                   // Warn that this is the debug build

    chibios_rt::BaseThread::setPriority(LOWPRIO);

    app::ROMBackend rom_backend;
    app::Platform platform;

    /*
     * Initializing the bootloader.
     * Using zero delay if everything is OK.
     * If we reset due to watchdog, add an extra delay to allow for intervention.
     */
    std::chrono::microseconds boot_delay(0);
    if (reset_cause == board::ResetCause::Watchdog)
    {
        std::puts("DEBRICKING");
        boot_delay = std::chrono::duration_cast<std::chrono::microseconds>(app::BootDelayAfterWatchdogTimedOut);
    }

    // This class is large, do not allocate it on the stack
    static kocherga::BootloaderController bl(platform,
                                             rom_backend,
                                             board::getFlashSize(),
                                             boot_delay);

    const auto apsh = app_shared::readAndInvalidateSharedStruct();

    /*
     * Bypass interface initialization and do a direct boot if the application is valid.
     * This is needed to minimize start-up latency.
     */
    if (apsh && apsh->stay_in_bootloader)
    {
        bl.cancelBoot();
    }

    if (bl.getState() == kocherga::State::ReadyToBoot)
    {
        // We haven't launched any components yet, so there's nothing to shut down;
        // but we have to uphold the protocol regardless.
        os::requestShutdown();
        app::doBoot();
    }

    /*
     * Serial init
     */
    board::kickWatchdog();

    board::usb::init();         // This is very slow, which is why we only init it if we're not booting immediately

    serial::init(bl);

    board::kickWatchdog();

    /*
     * UAVCAN init - depends on app shared.
     */
    if (apsh)
    {
        // Ensuring the string is properly terminated:
        char tmp[app_shared::AppShared::UAVCANFileNameCapacity]{};
        std::copy(&apsh->uavcan_file_name[0],
                  &apsh->uavcan_file_name[app_shared::AppShared::UAVCANFileNameCapacity - 1],
                  &tmp[0]);
        tmp[app_shared::AppShared::UAVCANFileNameCapacity - 1] = '\0';

        std::printf("AppShared: UAVCAN %u bps %u/%u \"%s\"\n",
                    unsigned(apsh->can_bus_speed),
                    apsh->uavcan_node_id,
                    apsh->uavcan_fw_server_node_id,
                    &tmp[0]);

        uavcan::init(bl,
                     apsh->can_bus_speed,
                     apsh->uavcan_node_id,
                     apsh->uavcan_fw_server_node_id,
                     &tmp[0]);
    }
    else
    {
        uavcan::init(bl);
    }

    /*
     * Main loop
     */
    while (!os::isShutdownRequested())
    {
        board::kickWatchdog();

        const auto bl_state = bl.getState();
        if (bl_state == kocherga::State::ReadyToBoot)
        {
            break;
        }

        app::setStatusLEDFromBootloaderState(bl_state);

        chThdSleepMilliseconds(50);
    }

    if (os::isShutdownRequested())
    {
        std::puts("REBOOT");
        chThdSleepMilliseconds(500);            // Providing some time for other components to react
        board::restart();
    }

    /*
     * Starting the application
     */
    os::requestShutdown();
    std::puts("BOOT");
    chThdSleepMilliseconds(500);                // Providing some time for other components to react
    app::doBoot();

    return 0;
}
