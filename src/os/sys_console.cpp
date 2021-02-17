/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#include "os.hpp"
#include <ch.hpp>
#include <hal.h>
#include <chprintf.h>
#include <memstreams.h>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdarg>


namespace os
{

static chibios_rt::Mutex g_mutex;

// Sink invocations are guaranteed to be protected by the mutex, so no extra locking is needed.
static bool sink(const std::uint8_t* const data, const std::size_t sz)
{
    return chnWriteTimeout(&STDOUT_SD, data, sz, TIME_MS2I(10)) == sz;
}

static std::size_t writeExpandingCrLf(const char* str)
{
    std::size_t ret = 0;
    const char* end = str;

    while (*str != '\0')
    {
        if ((*end == '\n') ||
            (*end == '\0'))
        {
            if (end != str)
            {
                const std::size_t range = end - str;
                if (!sink(reinterpret_cast<const std::uint8_t*>(str), range))
                {
                    break;
                }

                ret += range;
                str += range;
            }

            if (*end == '\n')
            {
                if (!sink(reinterpret_cast<const std::uint8_t*>("\r\n"), 2))
                {
                    break;
                }

                ret += 2U;
                str++;
            }
        }

        end++;
    }

    return ret;
}

static std::size_t genericPrint(const char* format, va_list vl)
{
    MutexLocker locker(g_mutex);

    /*
     * Printing the string into the buffer
     */
    char buffer[256]{};
    MemoryStream ms;
    msObjectInit(&ms, (uint8_t*)buffer, sizeof(buffer), 0);
    ::BaseSequentialStream* chp = (::BaseSequentialStream*)&ms;
    chvprintf(chp, format, vl);
    buffer[sizeof(buffer) - 1] = 0;     // Paranoid termination

    /*
     * Writing the buffer replacing "\n" --> "\r\n"
     */
    return writeExpandingCrLf(buffer);
}

} // namespace os

extern "C"
{

using namespace os;

int printf(const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    int ret = int(genericPrint(format, vl));
    va_end(vl);
    return ret;
}

int vprintf(const char* format, va_list vl)
{
    return int(genericPrint(format, vl));
}

int puts(const char* str)
{
    MutexLocker locker(g_mutex);
    int ret = int(writeExpandingCrLf(str));
    ret += int(writeExpandingCrLf("\n"));
    return ret;
}

}
