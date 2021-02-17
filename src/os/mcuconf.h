/**
 * Copyright (c) 2016-2017  Zubax Robotics  <info@zubax.com>
 */

/*
 * Based on ChibiOS demo application for STM32F4.
 *
 * Configuration for unused peripherals is not provided here.
 */

#ifndef _MCUCONF_H_
#define _MCUCONF_H_

/*
 * STM32F4xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

#define STM32F4xx_MCUCONF

#define STM32_HSECLK                        (8 * 1000 * 1000)

/*
 * HAL driver system settings.
 */
#define STM32_NO_INIT                       FALSE
#define STM32_HSI_ENABLED                   TRUE
#define STM32_LSI_ENABLED                   TRUE
#define STM32_HSE_ENABLED                   TRUE
#define STM32_LSE_ENABLED                   FALSE
#define STM32_CLOCK48_REQUIRED              TRUE
#define STM32_SW                            STM32_SW_PLL

#define STM32_PLLSRC                        STM32_PLLSRC_HSE
#define STM32_PLLM_VALUE                    4
#define STM32_PLLN_VALUE                    180
#define STM32_PLLP_VALUE                    2
#define STM32_PLLQ_VALUE                    7
// PLL I2S is disabled; output clocks are set to the minimum
#define STM32_PLLI2SM_VALUE                 4
#define STM32_PLLI2SN_VALUE                 180
#define STM32_PLLI2SR_VALUE                 7
#define STM32_PLLI2SP_VALUE                 8
#define STM32_PLLI2SQ_VALUE                 15
// PLL SAI is used to clock USB via the 48 MHz domain
#define STM32_PLLSAIM_VALUE                 4
#define STM32_PLLSAIN_VALUE                 96
#define STM32_PLLSAIP_VALUE                 4
#define STM32_PLLSAIQ_VALUE                 15

// TODO: Use STM32_TIMPRE_ENABLE when supported! See http://www.chibios.com/forum/viewtopic.php?f=38&t=3870&p=32660
#define STM32_HPRE                          STM32_HPRE_DIV1
#define STM32_PPRE1                         STM32_PPRE1_DIV4
#define STM32_PPRE2                         STM32_PPRE2_DIV2
#define STM32_RTCSEL                        STM32_RTCSEL_LSI
#define STM32_RTCPRE_VALUE                  8
#define STM32_MCO1SEL                       STM32_MCO1SEL_HSI
#define STM32_MCO1PRE                       STM32_MCO1PRE_DIV1
#define STM32_MCO2SEL                       STM32_MCO2SEL_HSE
#define STM32_MCO2PRE                       STM32_MCO2PRE_DIV5
#define STM32_CK48MSEL                      STM32_CK48MSEL_PLLSAI
#define STM32_PVD_ENABLE                    FALSE
#define STM32_PLS                           STM32_PLS_LEV0
#define STM32_BKPRAM_ENABLE                 FALSE

#define STM32_VDD                           330

/*
 * Here we need a 32-bit timer for RTOS time tracking.
 */
#define STM32_ST_USE_TIMER                  2

/*
 * SERIAL driver system settings.
 */
#define STDOUT_SD                           SD3
#define STDIN_SD                            STDOUT_SD

#define STM32_SERIAL_USE_USART1             FALSE
#define STM32_SERIAL_USE_USART2             FALSE
#define STM32_SERIAL_USE_USART3             TRUE
#define STM32_SERIAL_USE_UART4              FALSE
#define STM32_SERIAL_USE_UART5              FALSE
#define STM32_SERIAL_USE_USART6             FALSE
#define STM32_SERIAL_USART1_PRIORITY        12
#define STM32_SERIAL_USART2_PRIORITY        12
#define STM32_SERIAL_USART3_PRIORITY        12
#define STM32_SERIAL_UART4_PRIORITY         12
#define STM32_SERIAL_UART5_PRIORITY         12
#define STM32_SERIAL_USART6_PRIORITY        12

/*
 * USB driver system settings.
 * Note that the USB thread is doing exactly nothing unless USB is connected,
 * so it is safe to assign a very high priority here.
 * High priority is required for the real-time variable tracer.
 */
#define STM32_USB_USE_OTG1                  TRUE
#define STM32_USB_OTG1_IRQ_PRIORITY         5
#define STM32_USB_OTG1_RX_FIFO_SIZE         512
#define STM32_USB_OTG_THREAD_PRIO           (NORMALPRIO + 20)
#define STM32_USB_OTG_THREAD_STACK_SIZE     256
#define STM32_USB_OTGFIFO_FILL_BASEPRI      0

#endif /* _MCUCONF_H_ */
