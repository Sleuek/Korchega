/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#pragma once

#include <os.hpp>
#include <hal.h>
#include <cassert>
#include <cstring>
#include <cstdint>
#include <algorithm>

#if !defined(FLASH_SR_WRPRTERR) // Compatibility
# define FLASH_SR_WRPRTERR      FLASH_SR_WRPERR
#endif

namespace board
{
/**
 * The code below assumes that HSI oscillator is up and running,
 * otherwise the Flash controller (FPEC) may misbehave.
 * Any FPEC issues will be detected at run time during write/erase verification.
 */
class SequentialROMWriter
{
    std::size_t address_;
    std::uint8_t next_sector_to_erase_;

    static void waitReady()
    {
        do
        {
            assert(!(FLASH->SR & FLASH_SR_WRPRTERR));
            assert(!(FLASH->SR & FLASH_SR_PGAERR));
    #if !defined(STM32L496xx)
            assert(!(FLASH->SR & FLASH_SR_PGPERR));
    #endif
            assert(!(FLASH->SR & FLASH_SR_PGSERR));
        }
        while (FLASH->SR & FLASH_SR_BSY);
        FLASH->SR |= FLASH_SR_EOP;
    }

    struct Prologuer
    {
        const os::CriticalSectionLocker locker_;

        Prologuer()
        {
            waitReady();
            if (FLASH->CR & FLASH_CR_LOCK)
            {
                FLASH->KEYR = 0x45670123UL;
                FLASH->KEYR = 0xCDEF89ABUL;
            }
 #if !defined(STM32L496xx)
            FLASH->SR |= FLASH_SR_EOP | FLASH_SR_WRPRTERR | FLASH_SR_PGAERR | FLASH_SR_PGPERR | FLASH_SR_PGSERR;
#else
            FLASH->SR |= FLASH_SR_EOP | FLASH_SR_WRPRTERR | FLASH_SR_PGAERR | FLASH_SR_PGSERR;
#endif
            FLASH->CR = 0;
        }

        ~Prologuer()
        {
            FLASH->CR = FLASH_CR_LOCK;  // Reset the FPEC configuration and lock
        }
    };

    /**
     * This function maps an arbitrary address onto a flash sector number.
     * It need not be implemented for MCU which do not require sector numbers for operations on flash.
     * Returns negative if there's no match.
     */
    static std::optional<std::uint8_t> mapAddressToSectorNumber(const std::size_t where)
    {
        if (where < 0x08000000)
        {
            return {};
        }

        // 16K
        if (where <= 0x08003FFF) { return 0; }
        if (where <= 0x08007FFF) { return 1; }
        if (where <= 0x0800BFFF) { return 2; }
        if (where <= 0x0800FFFF) { return 3; }
        // 64K
        if (where <= 0x0801FFFF) { return 4; }
        // 128K
        if (where <= 0x0803FFFF) { return 5; }
        if (where <= 0x0805FFFF) { return 6; }
        if (where <= 0x0807FFFF) { return 7; }

        return {};
    }

    static void eraseSector(std::uint8_t sector_index)
    {
        Prologuer prologuer;
#if defined(STM32L496xx)
        FLASH->CR = FLASH_CR_PER | (sector_index << 3);
#else
        FLASH->CR = FLASH_CR_SER | (sector_index << 3);
#endif
        FLASH->CR |= FLASH_CR_STRT;
        waitReady();
        FLASH->CR = 0;
    }

public:
    enum class AutoEraseMode
    {
        Enabled,
        Disabled
    };

    /**
     * The address must be aligned at two bytes.
     */
    explicit SequentialROMWriter(std::size_t begin_address,
                                 AutoEraseMode auto_erase = AutoEraseMode::Enabled) :
        address_(begin_address),
        next_sector_to_erase_((auto_erase == AutoEraseMode::Enabled) ? 0 : 0xFF)
    {
        assert((address_ % 2U) == 0);
    }

    /**
     * The source address and the length must be aligned at two bytes.
     * The memory will be erased beforehand automatically as necessary.
     */
    bool append(const void* const what,
                const std::size_t how_much)
    {
        if (((reinterpret_cast<std::size_t>(what)) % 2 != 0) || (what == nullptr))
        {
            assert(false);
            return false;
        }

        const std::size_t original_address = address_;

        /*
         * Erase the sectors that we're going to write into beforehand.
         * Advance the erase sector index as we go - we don't want to erase sectors more than once because
         * that would destroy data that we've written earlier.
         * Note that we use granular critical sections to reduce IRQ impact.
         */
        for (std::size_t offset = 0; offset < how_much; offset++)
        {
            const auto sn = mapAddressToSectorNumber(address_);
            address_++;

            if (!sn)
            {
                return false;
            }

            if (*sn >= next_sector_to_erase_)
            {
                next_sector_to_erase_ = *sn;

                DEBUG_LOG("Erasing sector %d @%08x\n", next_sector_to_erase_, unsigned(address_));
                eraseSector(next_sector_to_erase_);

                next_sector_to_erase_++;
            }
        }

        /*
         * Now the memory is ready to be written. Run the write loop, two bytes per iteration.
         * We use a single big critical section to speed things up - writes are fast enough as they are.
         */
        const std::size_t num_halfwords = (how_much + 1U) / 2U;

        volatile std::uint16_t* flashptr16 = reinterpret_cast<std::uint16_t*>(original_address);
        const std::uint16_t* ramptr16 = static_cast<const std::uint16_t*>(what);

        {
            Prologuer prologuer;

#if defined(STM32L496xx)
            FLASH->CR = FLASH_CR_PG;
#else
            FLASH->CR = FLASH_CR_PG | FLASH_CR_PSIZE_0;
#endif

            for (std::size_t i = 0; i < num_halfwords; i++)
            {
                *flashptr16++ = *ramptr16++;
                waitReady();
            }

            waitReady();
            FLASH->CR = 0;
        }

        /*
         * Final verification - compare the memory contents with the original data.
         */
        return std::memcmp(what, reinterpret_cast<void*>(original_address), how_much) == 0;
    }

    void skip(const std::size_t how_much)
    {
        address_ += how_much;
    }

    std::size_t getAddress() const { return address_; }
};

}
