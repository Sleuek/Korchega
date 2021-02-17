/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#pragma once

#include <os.hpp>
#include <kocherga/kocherga.hpp>
#include <cstdint>


namespace serial
{
/**
 * Starts the firmware update endpoint.
 */
void init(kocherga::BootloaderController& bl);

}
