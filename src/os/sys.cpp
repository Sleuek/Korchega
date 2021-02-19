/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#include "os.hpp"
#include <chprintf.h>
#include <ch.hpp>
#include <unistd.h>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdarg>
#include <type_traits>
#include <board/board.hpp>


namespace os
{

static volatile bool g_shutdown_request_flag = false;

void requestShutdown()
{
    g_shutdown_request_flag = true;
}

bool isShutdownRequested()
{
    return g_shutdown_request_flag;
}

/**
 * Last-resort printing - bypassess all stdout logic and prints directly into the default CLI UART.
 * Can be used ONLY for panic reports.
 */
static void emergencyPrint(const char* str)
{
    for (const char *p = str; *p; p++)
    {
    #if defined(STM32L496xx)
        while (!(STDOUT_SD.usart->ISR & USART_ISR_TXE)) { }
        STDOUT_SD.usart->RDR = *p;
    #else
        while (!(STDOUT_SD.usart->SR & USART_SR_TXE)) { }
        STDOUT_SD.usart->DR = *p;
    #endif
    }
}

void sleepUntilChTime(systime_t sleep_until)
{
    chSysLock();
    sleep_until -= chVTGetSystemTimeX();
    if (static_cast<std::make_signed<systime_t>::type>(sleep_until) > 0)
    {
        chThdSleepS(sleep_until);
    }
    chSysUnlock();

#if defined(DEBUG_BUILD) && DEBUG_BUILD
    if (static_cast<std::make_signed<systime_t>::type>(sleep_until) < 0)
    {
        DEBUG_LOG("Lag %d ts\n", static_cast<int>(static_cast<std::make_signed<systime_t>::type>(sleep_until)));
    }
#endif
}

} // namespace os

extern "C"
{

__attribute__((weak))
void* __dso_handle;

__attribute__((weak))
int* __errno()
{
    static int en;
    return &en;
}


void systemHaltHook(const char* msg)
{
    board::setRGBLED(1.0F, 0, 0);

    port_disable();
    os::emergencyPrint("\r\nPANIC: ");
    if (msg != NULL)
    {
        os::emergencyPrint(msg);
    }
    os::emergencyPrint("\r\n");

#if defined(DEBUG_BUILD) && DEBUG_BUILD
    if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
    {
        __asm volatile ("bkpt #0\n"); // Break into the debugger
    }
#endif
}

/**
 * Overrides the weak handler defined in the OS.
 * This is required because the weak handler doesn't halt the OS, which is very dangerous!
 * More context: http://www.chibios.com/forum/viewtopic.php?f=35&t=3819&p=28555#p28555
 */
void _unhandled_exception()
{
    chSysHalt("UNDEFINED IRQ");
}


void __assert_func(const char* file, int line, const char*, const char*)
{
    port_disable();

    // We're using static here in order to avoid overflowing the stack in the event of assertion panic
    // Keeping the stack intact allows us to connect a debugger later and observe the state postmortem
    static char buffer[200]{};
    chsnprintf(&buffer[0], sizeof(buffer), "%s:%d", file, line);
    chSysHalt(&buffer[0]);

    while (true) { }
}

/// From unistd
int usleep(useconds_t useconds)
{
    assert((((uint64_t)useconds * (uint64_t)CH_CFG_ST_FREQUENCY + 999999ULL) / 1000000ULL)
           < (1ULL << CH_CFG_ST_RESOLUTION));
    // http://pubs.opengroup.org/onlinepubs/7908799/xsh/usleep.html
    if (useconds > 0)
    {
        chThdSleepMicroseconds(useconds);
    }
    return 0;
}

/// From unistd
unsigned sleep(unsigned int seconds)
{
    assert(((uint64_t)seconds * (uint64_t)CH_CFG_ST_FREQUENCY) < (1ULL << CH_CFG_ST_RESOLUTION));
    // http://pubs.opengroup.org/onlinepubs/7908799/xsh/sleep.html
    if (seconds > 0)
    {
        chThdSleepSeconds(seconds);
    }
    return 0;
}

void* malloc(size_t sz)
{
    (void) sz;
    assert(sz == 0);                    // We want debug builds to fail loudly; release builds are given a pass
    return nullptr;
}

void* calloc(size_t num, size_t sz)
{
    (void) num;
    (void) sz;
    assert((num == 0) || (sz == 0));    // We want debug builds to fail loudly; release builds are given a pass
    return nullptr;
}

void* realloc(void*, size_t sz)
{
    (void) sz;
    assert(sz == 0);                    // We want debug builds to fail loudly; release builds are given a pass
    return nullptr;
}

void free(void* p)
{
    /*
     * Certain stdlib functions, like mktime(), may call free() with zero argument, which can be safely ignored.
     */
    if (p != nullptr)
    {
        chSysHalt("free");
    }
}

void NMI_Handler(void)
{
    chibios_rt::System::halt("NMI");
}

void HardFault_Handler(void)
{
    chibios_rt::System::halt("HardFault");
}

void MemManage_Handler(void)
{
    chibios_rt::System::halt("MemManage");
}

void BusFault_Handler(void)
{
    chibios_rt::System::halt("BusFault");
}

void UsageFault_Handler(void)
{
    chibios_rt::System::halt("UsageFault");
}

}  // extern "C"
