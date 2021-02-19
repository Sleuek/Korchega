/**
 * Copyright (c) 2016-2017  Zubax Robotics  <info@zubax.com>
 */

#pragma once

#include <hal.h>
#include <array>
#include <cstdint>



namespace board
{
namespace usb
{
/**
 * Takes about one second to complete!
 */
void init();

SerialUSBDriver* getSerialUSBDriver();

enum class State
{
    Disconnected,
    Connected
};

State getState();

}
}
