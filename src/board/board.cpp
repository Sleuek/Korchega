/**
 * Copyright (c) 2018  Zubax Robotics  <info@zubax.com>
 */

#include "board.hpp"
#include "sequential_rom_writer.hpp"
#include <ch.hpp>
#include <cstring>
#include <unistd.h>

/// Provided by the linker
const extern std::uint8_t DeviceSignatureStorage[];

namespace board
{
namespace
{

inline void initLEDPWM()
{
    {
        os::CriticalSectionLocker cs;
        // Power-on and reset
#if defined(STM32L496xx)
        RCC->APB1ENR1  |=  RCC_APB1ENR1_TIM5EN;
        RCC->APB1RSTR1 |=  RCC_APB1RSTR1_TIM5RST;
        RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_TIM5RST;
#else
        RCC->APB1ENR  |=  RCC_APB1ENR_TIM5EN;
        RCC->APB1RSTR |=  RCC_APB1RSTR_TIM5RST;
        RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM5RST;
#endif
    }

    static constexpr unsigned FrequencyDivisionRatio = 10;

    TIM5->PSC = FrequencyDivisionRatio - 1U;
    TIM5->ARR = 0xFFFF;
    TIM5->CR1 = 0;
    TIM5->CR2 = 0;

    // CC2, CC3, CC4 are R, G, B. Inverted mode.
    TIM5->CCMR1 = TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
    TIM5->CCMR2 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 |
                  TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;

    // All enabled, all inverted.
    TIM5->CCER = TIM_CCER_CC4E | TIM_CCER_CC3E | TIM_CCER_CC2E |
                 TIM_CCER_CC4P | TIM_CCER_CC3P | TIM_CCER_CC2P;

    // Start
    TIM5->EGR = TIM_EGR_UG | TIM_EGR_COMG;
    TIM5->CR1 |= TIM_CR1_CEN;
}


inline void initCAN()
{
    os::CriticalSectionLocker locker;

 #if defined(STM32L496xx)
    RCC->APB1ENR1  |=  RCC_APB1ENR1_CAN1EN;           // CAN1
    RCC->APB1RSTR1 |=  RCC_APB1RSTR1_CAN1RST;
    RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_CAN1RST;
#else
    RCC->APB1ENR  |=  RCC_APB1ENR_CAN1EN;           // CAN1
    RCC->APB1RSTR |=  RCC_APB1RSTR_CAN1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN1RST;
#endif
    // CAN2 is not used
}


inline std::uint8_t readHardwareID()
{
     #if defined(STM32L496xx)
        return std::uint8_t((PAL_LOW << 0) |
                            (PAL_LOW << 1) |
                            (PAL_LOW << 2));
    #else
        return std::uint8_t((palReadPad(GPIOC, GPIOC_HARDWARE_ID_BIT_0) << 0) |
                            (palReadPad(GPIOC, GPIOC_HARDWARE_ID_BIT_1) << 1) |
                            (palReadPad(GPIOC, GPIOC_HARDWARE_ID_BIT_2) << 2));
    #endif
    

    
}

}   // namespace


Clock::time_point Clock::now() noexcept
{
    static ::systime_t prev_sample = 0;
    static std::uint64_t base = 0;

    os::CriticalSectionLocker locker;

#ifndef NDEBUG
    const auto old_base = base;
#endif

    // Computing increment since last invocation
    const ::systime_t ts = chVTGetSystemTimeX();
    const std::uint64_t increment = ::systime_t(ts - prev_sample);
    prev_sample = ts;

    // Add the increment to the absolute time estimate
    base += increment;

    // Check correctness by making sure the time doesn't go backwards
    assert(old_base <= base);

    return time_point(duration(base));
}


ResetCause init(std::chrono::milliseconds watchdog_timeout)
{
    /*
     * OS initialization first
     */
    halInit();
    chibios_rt::System::init();

    /*
     * Serial port - should be initialized early because it is used to report debug data
     */
    sdStart(&STDOUT_SD, nullptr);

    /*
     * Watchdog
     * We do not enable it in debug builds in order to not interfere with debugging
     */
    RCC->CSR |= RCC_CSR_LSION;                  // Make sure LSI is enabled
    while (!(RCC->CSR & RCC_CSR_LSIRDY)) { }    // Wait for LSI startup

#if !defined(DEBUG_BUILD) || (DEBUG_BUILD == 0)
    IWDG->KR = 0xAAAA;
    while (IWDG->SR != 0) { }                   // Wait until the IWDG is ready to accept the new parameters
    IWDG->KR = 0x5555;
    IWDG->PR = 6;                               // Div 256 yields 6.4ms per clock period at 40kHz
    IWDG->RLR = std::uint16_t(std::min<std::int64_t>(watchdog_timeout.count() / 6, 0x0FFF));
    IWDG->KR = 0xAAAA;
    IWDG->KR = 0xCCCC;
#else
    (void) watchdog_timeout;
#endif

    ResetCause reset_cause = ResetCause::Other;

    const std::uint32_t rcc_csr = RCC->CSR;
    RCC->CSR |= RCC_CSR_RMVF;                   // Clear reset cause flags

    if ((rcc_csr & (RCC_CSR_IWDGRSTF | RCC_CSR_WWDGRSTF)) != 0)         // Check for either watchdog
    {
        reset_cause = ResetCause::Watchdog;
    }

    /*
     * Interfaces
     */
    initLEDPWM();

    initCAN();

    kickWatchdog();

    return reset_cause;
}

void kickWatchdog()
{
    IWDG->KR = 0xAAAA;
}

[[noreturn]]
void die(int reason)
{
    chibios_rt::System::halt(senoval::convertIntToString(reason));
    while (1)
    {
    }
}

[[noreturn]]
void bootApplication()
{
    // Loading and validating the application entry point
    const unsigned stacktop   = *reinterpret_cast<unsigned*>(FLASH_BASE + APPLICATION_OFFSET);
    const unsigned entrypoint = *reinterpret_cast<unsigned*>(FLASH_BASE + APPLICATION_OFFSET + 4);

    if ((stacktop <= SRAM_BASE) ||
        (stacktop > (SRAM_BASE + 2 * 1024 * 1024)))
    {
        chibios_rt::System::halt("STACKTOP");
    }

    if ((entrypoint < (FLASH_BASE + APPLICATION_OFFSET)) ||
        (entrypoint >= (FLASH_BASE + getFlashSize())))
    {
        chibios_rt::System::halt("ENTRYPOINT");
    }

    // We cordially extend our thanks to David Sidrane and Ben Dyer, whose ideas have somewhat inspired this thing.
    asm volatile("cpsid i");

    // Deinit all peripherals that may have been used
    // It is crucial to disable ALL peripherals, else a spurious interrupt will crash the application
#if defined(STM32L496xx)
    RCC->APB1RSTR1 |= (RCC_APB1RSTR1_CAN1RST | RCC_APB1RSTR1_USART3RST);
# if UAVCAN_STM32_NUM_IFACES > 1
    RCC->APB1RSTR1 |=  RCC_APB1RSTR1_CAN2RST;
# endif
#else
    RCC->APB1RSTR |=  (RCC_APB1RSTR_CAN1RST | RCC_APB1RSTR_CAN2RST | RCC_APB1RSTR_USART3RST);
# if UAVCAN_STM32_NUM_IFACES > 1
    RCC->APB1RSTR1 |=  RCC_APB1RSTR1_CAN2RST;
# endif
#endif
    RCC->AHB2RSTR |=  RCC_AHB2RSTR_OTGFSRST;

    // Kill the sys tick
    SysTick->CTRL = 0;

    // Update the vector table location
    asm volatile("dsb");
    asm volatile("isb");
    SCB->VTOR = FLASH_BASE + APPLICATION_OFFSET;
    asm volatile("dsb");

    // Let's roll!
    asm volatile("msr msp, %[stacktop]          \n"
                 "bx       %[entrypoint]        \n"
                 :: [stacktop] "r"(stacktop), [entrypoint] "r"(entrypoint):);

    for (;;) { }        // Noreturn
}

[[noreturn]]
void restart()
{
    NVIC_SystemReset();
    for (;;) { }        // Noreturn
}

UniqueID readUniqueID()
{
    UniqueID bytes;
    std::fill(bytes.begin(), bytes.end(), 0);
    std::memcpy(bytes.data(), reinterpret_cast<const void*>(0x1FFF7A10), 12);
    return bytes;
}

std::optional<DeviceSignature> tryReadDeviceSignature()
{
    // Read from the designated storage in ROM
    DeviceSignature out_sign{};
    std::memcpy(out_sign.data(), &DeviceSignatureStorage[0], out_sign.size());

    // Verify if it looks valid
    for (auto x : out_sign)
    {
        if ((x != 0xFF) && (x != 0x00))          // All 0xFF/0x00 is not a valid signature, it's empty storage
        {
            return out_sign;
        }
    }

    return {};
}

bool tryWriteDeviceSignature(const DeviceSignature& sign)
{
    SequentialROMWriter writer(reinterpret_cast<std::size_t>(&DeviceSignatureStorage[0]),
                               SequentialROMWriter::AutoEraseMode::Disabled);
    return writer.append(sign.data(), sign.size());
}

HardwareVersion detectHardwareVersion()
{
    static const HardwareVersion v
    {
        HW_VERSION_MAJOR,
        readHardwareID()
    };
    return v;
}

void setRGBLED(float red, float green, float blue)
{
    constexpr unsigned Multiplier = 0xFFFF;
    TIM5->CCR2 = unsigned(red   * Multiplier + 0.4F);
    TIM5->CCR3 = unsigned(green * Multiplier + 0.4F);
    TIM5->CCR4 = unsigned(blue  * Multiplier + 0.4F);
}

void setCANActivityLED(const int interface_index, const bool state)
{
    if (interface_index == 0)
    {
    #if defined(STM32L496xx)
        palWriteLine(LINE_LED_SYSTEM, unsigned(!state));
    #else
        palWritePad(GPIOB, GPIOB_CAN1_LED_INVERSE, unsigned(!state));
    #endif
    
    }
    else if (interface_index == 1)
    {
    #if defined(STM32L496xx)
         palWriteLine(LINE_LED_ALIVE, unsigned(!state));
    #else
         palWritePad(GPIOA, GPIOA_CAN2_LED_INVERSE, unsigned(!state));
    #endif
       
    }
    else
    {
        assert(false);
    }
}

void setCANBusPowerOutputEnabled(bool enabled)
{
    #if defined(STM32L496xx)
        palWritePad(GPIOD, GPIOD_CAN_POWER_OUTPUT_ENABLE, unsigned(enabled));
    #else
         palWritePad(GPIOD, GPIOD_CAN_POWER_OUTPUT_ENABLE, unsigned(enabled));
    #endif
    
}

} // namespace board

/*
 * Early init from ChibiOS
 */
extern "C"
{

static void initGPIO(void)
{
    // Enabling GPIO-related clocks, the mask comes from the registry header file.
    rccResetAHB1(STM32_GPIO_EN_MASK);
    rccEnableAHB1(STM32_GPIO_EN_MASK, true);

    static const auto hit = [](stm32_gpio_t* const gpiop,
                               std::uint32_t moder,
                               std::uint32_t otyper,
                               std::uint32_t ospeedr,
                               std::uint32_t pupdr,
                               std::uint32_t odr,
                               std::uint32_t afrl,
                               std::uint32_t afrh)
        {
            gpiop->OTYPER  = otyper;
            gpiop->OSPEEDR = ospeedr;
            gpiop->PUPDR   = pupdr;
            gpiop->ODR     = odr;
            gpiop->AFRL    = afrl;
            gpiop->AFRH    = afrh;
            gpiop->MODER   = moder;
        };

#define INIT_GPIO_HIT_ONCE(x)           \
    hit(GPIO##x,                        \
        VAL_GPIO##x##_MODER,            \
        VAL_GPIO##x##_OTYPER,           \
        VAL_GPIO##x##_OSPEEDR,          \
        VAL_GPIO##x##_PUPDR,            \
        VAL_GPIO##x##_ODR,              \
        VAL_GPIO##x##_AFRL,             \
        VAL_GPIO##x##_AFRH);

    INIT_GPIO_HIT_ONCE(A);
    INIT_GPIO_HIT_ONCE(B);
    INIT_GPIO_HIT_ONCE(C);
    INIT_GPIO_HIT_ONCE(D);
    INIT_GPIO_HIT_ONCE(E);
    INIT_GPIO_HIT_ONCE(F);
    INIT_GPIO_HIT_ONCE(G);
    INIT_GPIO_HIT_ONCE(H);
}

void __early_init(void)
{
    initGPIO();
    stm32_clock_init();

     // CAN
#if defined(STM32L496xx)
    RCC->APB1RSTR1 |=  RCC_APB1RSTR1_CAN1RST;
    RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_CAN1RST;
# if UAVCAN_STM32_NUM_IFACES > 1
    RCC->APB1RSTR |=  RCC_APB1RSTR2_CAN2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR2_CAN2RST;
# endif
#else
    RCC->APB1RSTR |=  RCC_APB1RSTR_CAN1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN1RST;
# if UAVCAN_STM32_NUM_IFACES > 1
    RCC->APB1RSTR |=  RCC_APB1RSTR_CAN2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN2RST;
# endif
#endif

    CAN1->IER = 0;                                  // Disable interrupts
    CAN1->MCR = CAN_MCR_SLEEP | CAN_MCR_RESET;      // Software reset
#if UAVCAN_STM32_NUM_IFACES > 1
    CAN2->IER = 0;
    CAN2->MCR = CAN_MCR_SLEEP | CAN_MCR_RESET;
#endif

    NVIC_ClearPendingIRQ(CAN1_RX0_IRQn);
    NVIC_ClearPendingIRQ(CAN1_RX1_IRQn);
    NVIC_ClearPendingIRQ(CAN1_TX_IRQn);
    NVIC_ClearPendingIRQ(CAN1_SCE_IRQn);

#if UAVCAN_STM32_NUM_IFACES > 1
    NVIC_ClearPendingIRQ(CAN2_RX0_IRQn);
    NVIC_ClearPendingIRQ(CAN2_RX1_IRQn);
    NVIC_ClearPendingIRQ(CAN2_TX_IRQn);
    NVIC_ClearPendingIRQ(CAN2_SCE_IRQn);
#endif

    // USB
    RCC->AHB2RSTR |=  RCC_AHB2RSTR_OTGFSRST;
    RCC->AHB2RSTR &= ~RCC_AHB2RSTR_OTGFSRST;

    NVIC_ClearPendingIRQ(OTG_FS_IRQn);
#if !defined(STM32L496xx)
    NVIC_ClearPendingIRQ(OTG_FS_WKUP_IRQn);
#endif
}

void boardInit(void)
{
}

}
