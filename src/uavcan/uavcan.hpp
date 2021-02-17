/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#pragma once

#include <os.hpp>
#include <kocherga/kocherga.hpp>
#include <cstdint>


namespace uavcan
{
/**
 * Starts the firmware update node. All parameters except the bootloader reference are optional.
 *
 * @param bl                        reference to the bootloader controller instance
 * @param can_bit_rate              bit rate of the CAN bus, if supplied by the application, otherwise 0
 * @param local_node_id             local node ID, if supplied by the application, otherwise 0
 * @param file_server_node_id       node ID of the remote node that will provide the file, if known, otherwise 0
 * @param remote_image_file_path    path of the firmware image file on the remote file server, if known, otherwise null
 */
void init(kocherga::BootloaderController& bl,
          const std::uint32_t can_bit_rate = 0,
          const std::uint8_t local_node_id = 0,
          const std::uint8_t file_server_node_id = 0,
          const char* const remote_image_file_path = nullptr);

/**
 * Runtime estimated UAVCAN bus parameters.
 * Unknown parameters are set to zero.
 */
struct Parameters
{
    std::uint32_t can_bus_bit_rate = 0;         ///< Set if CAN bus bit rate auto detection was successful
    std::uint8_t local_node_id = 0;             ///< Set if dynamic node ID allocation was successful
};

Parameters getParameters();

}
