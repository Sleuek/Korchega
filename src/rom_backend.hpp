/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#pragma once

#include "board/sequential_rom_writer.hpp"
#include "board/board.hpp"
#include <cstring>
#include <cstdlib>
#include <os.hpp>
#include <kocherga/kocherga.hpp>
#include <hal.h>


namespace app
{
/**
 * This class contains logic and hardcoded values that are SPECIFIC FOR THIS PARTICULAR MCU AND APPLICATION.
 */
class ROMBackend : public kocherga::IROMBackend
{
    std::optional<board::SequentialROMWriter> writer_;

    static constexpr std::size_t ApplicationAddress = FLASH_BASE + APPLICATION_OFFSET;

    static bool correctOffsetAndSize(std::size_t& offset, std::uint16_t& size)
    {
        const auto flash_end = FLASH_BASE + board::getFlashSize();
        offset += ApplicationAddress;
        if (offset >= flash_end)
        {
            return false;
        }
        if ((offset + size) >= flash_end)
        {
            size = std::uint16_t(flash_end - offset);
        }
        return true;
    }

public:
    std::int16_t beginUpgrade()   override
    {
        writer_.emplace(ApplicationAddress);
        return 0;
    }

    std::int16_t endUpgrade(bool) override
    {
        writer_.reset();
        return 0;
    }

    std::int16_t write(std::size_t offset, const void* data, std::uint16_t size) override
    {
        if (!writer_)
        {
            return 0;
        }

        if (!correctOffsetAndSize(offset, size))
        {
            return 0;
        }

        if (offset < writer_->getAddress())
        {
            DEBUG_LOG("Non-sequential writes are not supported: %08x < %08x\n",
                      unsigned(offset), unsigned(writer_->getAddress()));
            return 0;
        }

        if (offset > writer_->getAddress())
        {
            writer_->skip(offset - writer_->getAddress());
        }

        assert(offset == writer_->getAddress());
        return writer_->append(data, size) ? std::int16_t(size) : -1;
    }

    std::int16_t read(std::size_t offset, void* data, std::uint16_t size) const override
    {
        if (correctOffsetAndSize(offset, size))
        {
            std::memcpy(data, reinterpret_cast<const void*>(offset), size);
            return std::int16_t(size);
        }
        else
        {
            return 0;
        }
    }
};

}
