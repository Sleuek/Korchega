/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#pragma once

#include <os.hpp>
#include <senoval/string.hpp>
#include <hal.h>
#include <cstdint>
#include <chrono>
#include <array>


namespace board
{
/**
 * Simple monotonic clock used for timestamping; measures the time since boot, never overflows.
 * Implementation of the TrivialClock trait from std.
 *
 * Note that this implementation requires that the clock is sampled regularly,
 * otherwise the base timer may overflow and shift the epoch backwards!
 *
 * http://en.cppreference.com/w/cpp/concept/TrivialClock
 * http://en.cppreference.com/w/cpp/concept/Clock
 */
struct Clock
{
    using rep        = std::int64_t;
    using period     = std::ratio<1, CH_CFG_ST_FREQUENCY>;
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<Clock>;

    static constexpr bool is_steady = true;

    static time_point now() noexcept;
};

/**
 * Encodes the reason of the latest (current) reset
 */
enum class ResetCause : std::uint8_t
{
    Watchdog,
    Other
};

/**
 * This function should be invoked immediately from main().
 * It initializes a watchdog timer instance ASAP, which ensures that the application
 * will not be stuck forever if initialization fails.
 * Note that this function erases the reset cause flags from the RCC CSR register!
 */
ResetCause init(std::chrono::milliseconds watchdog_timeout);

/**
 * Resets the hardware watchdog.
 */
void kickWatchdog();

/**
 * Triggers an OS panic with the specified reason code printed into the serial console.
 */
[[noreturn]]
void die(int reason);

/**
 * Starts the application normally.
 * No validity checks of any kind are performed.
 */
[[noreturn]]
void bootApplication();

/**
 * Resets the MCU via NVIC, no additional actions are performed.
 */
[[noreturn]]
void restart();

/**
 * Returns the 128-bit hardware UID, where only the first 96 bit are used, and the rest is
 * filled with zeros.
 */
typedef std::array<std::uint8_t, 16> UniqueID;
UniqueID readUniqueID();

/**
 * RSA-1776 Signature of Authenticity management.
 * The signature can be written only once and it is typically done by the hardware manufacturer.
 * Read will fail if there is no signature installed. Write will fail if there is some signature already installed.
 */
typedef std::array<std::uint8_t, 222> DeviceSignature;
std::optional<DeviceSignature> tryReadDeviceSignature();
bool tryWriteDeviceSignature(const DeviceSignature& sign);

/**
 * @ref detectHardwareVersion()
 */
struct HardwareVersion
{
    std::uint8_t major;
    std::uint8_t minor;
};

/**
 * The major code can be specified either by the hardware ID pins or at compile time.
 * The minor code is specified only by the hardware ID pins.
 */
HardwareVersion detectHardwareVersion();

/**
 * Returns the flash size on this particular microcontroller.
 */
inline std::uint32_t getFlashSize()
{
    const std::uint32_t out = 1024U * *reinterpret_cast<std::uint16_t*>(0x1FFF7A22);

    // Sanity check
    assert(out >= 128 * 1024);
    assert(out <= 2048 * 1024);

    return out;
}

/**
 * Sets the LED color.
 */
void setRGBLED(float red, float green, float blue);

/**
 * Controls the CAN activity LED, per interface.
 */
void setCANActivityLED(const int interface_index, const bool state);

/**
 * Enables/disables the CAN bus power supply output.
 */
void setCANBusPowerOutputEnabled(bool enabled);

/**
 * Testpoint control.
 * The functions are made inline to ensure minimal runtime overhead even with LTO disabled.
 * Note that testpoint mapping depends on the hardware revision. Refer to the hardware design documentation for more
 * information.
 * @{
 */
template <unsigned TestPointIndex>
void setTestPoint(bool level);

template <>
inline void setTestPoint<0>(bool level)
{
    palWritePad(GPIOB, GPIOB_TEST_1, unsigned(level));
}

template <>
inline void setTestPoint<1>(bool level)
{
    palWritePad(GPIOB, GPIOB_TEST_2, unsigned(level));
}

/**
 * Use this helper to toggle testpoints:
 *      RAIITestPointToggler<0> toggler;
 */
template <unsigned TestPointIndex>
struct RAIITestPointTogglerImpl_
{
    RAIITestPointTogglerImpl_()  { setTestPoint<TestPointIndex>(true); }
    ~RAIITestPointTogglerImpl_() { setTestPoint<TestPointIndex>(false); }
};

template <unsigned TestPointIndex>
using RAIITestPointToggler = volatile RAIITestPointTogglerImpl_<TestPointIndex>;

}
