/**
 * Copyright (c) 2016-2017  Zubax Robotics  <info@zubax.com>
 */

#pragma once

#define STM32F446xx

/*
 * Pin definitions
 */
/*
 * Port A
 */
#define GPIOA_PHASE_VOLTAGE_FEEDBACK_A  0
#define GPIOA_PHASE_VOLTAGE_FEEDBACK_B  1
#define GPIOA_PHASE_VOLTAGE_FEEDBACK_C  2

#define GPIOA_MOTOR_TEMPERATURE_SENS    3
#define GPIOA_GPIO2                     4
#define GPIOA_TEST_DAC                  5
#define GPIOA_DC_CAL                    6

#define GPIOA_MOTOR_PWM_1N              7
#define GPIOA_MOTOR_PWM_1P              8
#define GPIOA_MOTOR_PWM_2P              9
#define GPIOA_MOTOR_PWM_3P              10

#define GPIOA_USB_DM                    11
#define GPIOA_USB_DP                    12

#define GPIOA_SWDIO                     13
#define GPIOA_SWCLK                     14

#define GPIOA_CAN2_LED_INVERSE          15

/*
 * Port B
 */
#define GPIOB_MOTOR_PWM_2N              0
#define GPIOB_MOTOR_PWM_3N              1

#define GPIOB_GAIN                      2

#define GPIOB_TEST_1                    3
#define GPIOB_TEST_2                    4

#define GPIOB_CAN2_RX                   5
#define GPIOB_CAN2_TX                   6

#define GPIOB_I2C_SDA                   7

#define GPIOB_CAN1_RX                   8
#define GPIOB_CAN1_TX                   9

#define GPIOB_UART_TX                   10
#define GPIOB_EN_GATE                   12
#define GPIOB_USB_VBUS_DETECT           13
#define GPIOB_RCPWM                     14
#define GPIOB_CAN1_LED_INVERSE          15

/*
 * Port C
 */
#define GPIOC_TEMPERATURE_SENS          0
#define GPIOC_VBAT_SENS                 1
#define GPIOC_CURRENT_2_SENS            2
#define GPIOC_CURRENT_1_SENS            3

#define GPIOC_MOTOR_TEMPERATURE_PULL_RES 4
#define GPIOC_UART_RX                   5
#define GPIOC_I2C_SCL                   6

#define GPIOC_RGB_LED_RED_INVERSE       7
#define GPIOC_RGB_LED_GREEN_INVERSE     8
#define GPIOC_RGB_LED_BLUE_INVERSE      9

#define GPIOC_HARDWARE_ID_BIT_2         10
#define GPIOC_HARDWARE_ID_BIT_1         11
#define GPIOC_HARDWARE_ID_BIT_0         12

#define GPIOC_LVPS_MALFUNCTION_INVERSE  13
#define GPIOC_VSI_OVERLOAD_INVERSE      14
#define GPIOC_VSI_FAULT_INVERSE         15

/*
 * Port D
 */
// The CAN power output is disabled while in the bootloader
#define GPIOD_CAN_POWER_OUTPUT_ENABLE   2

/*
 * Port E
 */
// Not present

/*
 * Port F
 */
// Not present

/*
 * Port G
 */
// Not present

/*
 * Port H
 */
#define GPIOH_OSC_IN                    0
#define GPIOH_OSC_OUT                   1

/*
 * I/O ports initial setup, this configuration is established soon after reset in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2U))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2U))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_PHASE_VOLTAGE_FEEDBACK_A) |\
                                     PIN_MODE_INPUT(GPIOA_PHASE_VOLTAGE_FEEDBACK_B) |\
                                     PIN_MODE_INPUT(GPIOA_PHASE_VOLTAGE_FEEDBACK_C) |\
                                     PIN_MODE_ANALOG(GPIOA_MOTOR_TEMPERATURE_SENS) |\
                                     PIN_MODE_INPUT(GPIOA_GPIO2) |\
                                     PIN_MODE_ANALOG(GPIOA_TEST_DAC) |\
                                     PIN_MODE_INPUT(GPIOA_DC_CAL) |\
                                     PIN_MODE_INPUT(GPIOA_MOTOR_PWM_1N) |\
                                     PIN_MODE_INPUT(GPIOA_MOTOR_PWM_1P) |\
                                     PIN_MODE_INPUT(GPIOA_MOTOR_PWM_2P) |\
                                     PIN_MODE_INPUT(GPIOA_MOTOR_PWM_3P) |\
                                     PIN_MODE_ALTERNATE(GPIOA_USB_DM) |\
                                     PIN_MODE_ALTERNATE(GPIOA_USB_DP) |\
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |\
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |\
                                     PIN_MODE_OUTPUT(GPIOA_CAN2_LED_INVERSE))

#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_PHASE_VOLTAGE_FEEDBACK_A) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PHASE_VOLTAGE_FEEDBACK_B) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PHASE_VOLTAGE_FEEDBACK_C) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOA_MOTOR_TEMPERATURE_SENS) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOA_GPIO2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_TEST_DAC) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_DC_CAL) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_MOTOR_PWM_1N) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_MOTOR_PWM_1P) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_MOTOR_PWM_2P) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_MOTOR_PWM_3P) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_DM) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_DP) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOA_CAN2_LED_INVERSE))

#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_2M(GPIOA_PHASE_VOLTAGE_FEEDBACK_A) |\
                                     PIN_OSPEED_2M(GPIOA_PHASE_VOLTAGE_FEEDBACK_B) |\
                                     PIN_OSPEED_2M(GPIOA_PHASE_VOLTAGE_FEEDBACK_C) |\
                                     PIN_OSPEED_2M(GPIOA_MOTOR_TEMPERATURE_SENS) |\
                                     PIN_OSPEED_2M(GPIOA_GPIO2) |\
                                     PIN_OSPEED_2M(GPIOA_TEST_DAC) |\
                                     PIN_OSPEED_2M(GPIOA_DC_CAL) |\
                                     PIN_OSPEED_25M(GPIOA_MOTOR_PWM_1N) |\
                                     PIN_OSPEED_25M(GPIOA_MOTOR_PWM_1P) |\
                                     PIN_OSPEED_25M(GPIOA_MOTOR_PWM_2P) |\
                                     PIN_OSPEED_25M(GPIOA_MOTOR_PWM_3P) |\
                                     PIN_OSPEED_100M(GPIOA_USB_DM) |\
                                     PIN_OSPEED_100M(GPIOA_USB_DP) |\
                                     PIN_OSPEED_50M(GPIOA_SWDIO) |\
                                     PIN_OSPEED_50M(GPIOA_SWCLK) |\
                                     PIN_OSPEED_2M(GPIOA_CAN2_LED_INVERSE))

#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOA_PHASE_VOLTAGE_FEEDBACK_A) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_PHASE_VOLTAGE_FEEDBACK_B) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_PHASE_VOLTAGE_FEEDBACK_C) |\
                                     PIN_PUPDR_FLOATING(GPIOA_MOTOR_TEMPERATURE_SENS) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_GPIO2) |\
                                     PIN_PUPDR_FLOATING(GPIOA_TEST_DAC) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_DC_CAL) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_MOTOR_PWM_1N) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_MOTOR_PWM_1P) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_MOTOR_PWM_2P) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_MOTOR_PWM_3P) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_DM) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_DP) |\
                                     PIN_PUPDR_PULLUP(GPIOA_SWDIO) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_SWCLK) |\
                                     PIN_PUPDR_FLOATING(GPIOA_CAN2_LED_INVERSE))

#define VAL_GPIOA_ODR               (PIN_ODR_LOW(GPIOA_PHASE_VOLTAGE_FEEDBACK_A) |\
                                     PIN_ODR_LOW(GPIOA_PHASE_VOLTAGE_FEEDBACK_B) |\
                                     PIN_ODR_LOW(GPIOA_PHASE_VOLTAGE_FEEDBACK_C) |\
                                     PIN_ODR_LOW(GPIOA_MOTOR_TEMPERATURE_SENS) |\
                                     PIN_ODR_LOW(GPIOA_GPIO2) |\
                                     PIN_ODR_LOW(GPIOA_TEST_DAC) |\
                                     PIN_ODR_LOW(GPIOA_DC_CAL) |\
                                     PIN_ODR_LOW(GPIOA_MOTOR_PWM_1N) |\
                                     PIN_ODR_LOW(GPIOA_MOTOR_PWM_1P) |\
                                     PIN_ODR_LOW(GPIOA_MOTOR_PWM_2P) |\
                                     PIN_ODR_LOW(GPIOA_MOTOR_PWM_3P) |\
                                     PIN_ODR_HIGH(GPIOA_USB_DM) |\
                                     PIN_ODR_HIGH(GPIOA_USB_DP) |\
                                     PIN_ODR_HIGH(GPIOA_SWDIO) |\
                                     PIN_ODR_HIGH(GPIOA_SWCLK) |\
                                     PIN_ODR_HIGH(GPIOA_CAN2_LED_INVERSE))

#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_PHASE_VOLTAGE_FEEDBACK_A,  0) |\
                                     PIN_AFIO_AF(GPIOA_PHASE_VOLTAGE_FEEDBACK_B,  0) |\
                                     PIN_AFIO_AF(GPIOA_PHASE_VOLTAGE_FEEDBACK_C,  0) |\
                                     PIN_AFIO_AF(GPIOA_MOTOR_TEMPERATURE_SENS,  0) |\
                                     PIN_AFIO_AF(GPIOA_GPIO2,  0) |\
                                     PIN_AFIO_AF(GPIOA_TEST_DAC,  0) |\
                                     PIN_AFIO_AF(GPIOA_DC_CAL,  0) |\
                                     PIN_AFIO_AF(GPIOA_MOTOR_PWM_1N,  0) )
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_MOTOR_PWM_1P,  0) |\
                                     PIN_AFIO_AF(GPIOA_MOTOR_PWM_2P,  0) |\
                                     PIN_AFIO_AF(GPIOA_MOTOR_PWM_3P, 0) |\
                                     PIN_AFIO_AF(GPIOA_USB_DM, 10) |\
                                     PIN_AFIO_AF(GPIOA_USB_DP, 10) |\
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0) |\
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0) |\
                                     PIN_AFIO_AF(GPIOA_CAN2_LED_INVERSE, 0))

/*
 * GPIOB
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_MOTOR_PWM_2N) |\
                                     PIN_MODE_INPUT(GPIOB_MOTOR_PWM_3N) |\
                                     PIN_MODE_INPUT(GPIOB_GAIN) |\
                                     PIN_MODE_OUTPUT(GPIOB_TEST_1) |\
                                     PIN_MODE_OUTPUT(GPIOB_TEST_2) |\
                                     PIN_MODE_ALTERNATE(GPIOB_CAN2_RX) |\
                                     PIN_MODE_ALTERNATE(GPIOB_CAN2_TX) |\
                                     PIN_MODE_INPUT(GPIOB_I2C_SDA) |\
                                     PIN_MODE_ALTERNATE(GPIOB_CAN1_RX) |\
                                     PIN_MODE_ALTERNATE(GPIOB_CAN1_TX) |\
                                     PIN_MODE_ALTERNATE(GPIOB_UART_TX) |\
                                     PIN_MODE_INPUT(11) |\
                                     PIN_MODE_OUTPUT(GPIOB_EN_GATE) |\
                                     PIN_MODE_INPUT(GPIOB_USB_VBUS_DETECT) |\
                                     PIN_MODE_ANALOG(GPIOB_RCPWM) |\
                                     PIN_MODE_OUTPUT(GPIOB_CAN1_LED_INVERSE))

#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_MOTOR_PWM_2N) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_MOTOR_PWM_3N) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_GAIN) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_TEST_1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_TEST_2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_CAN2_RX) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_CAN2_TX) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C_SDA) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_CAN1_RX) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_CAN1_TX) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_UART_TX) |\
                                     PIN_OTYPE_PUSHPULL(11) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_EN_GATE) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_USB_VBUS_DETECT) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_RCPWM) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_CAN1_LED_INVERSE))

#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_25M(GPIOB_MOTOR_PWM_2N) |\
                                     PIN_OSPEED_25M(GPIOB_MOTOR_PWM_3N) |\
                                     PIN_OSPEED_2M(GPIOB_GAIN) |\
                                     PIN_OSPEED_25M(GPIOB_TEST_1) |\
                                     PIN_OSPEED_25M(GPIOB_TEST_2) |\
                                     PIN_OSPEED_25M(GPIOB_CAN2_RX) |\
                                     PIN_OSPEED_25M(GPIOB_CAN2_TX) |\
                                     PIN_OSPEED_2M(GPIOB_I2C_SDA) |\
                                     PIN_OSPEED_25M(GPIOB_CAN1_RX) |\
                                     PIN_OSPEED_25M(GPIOB_CAN1_TX) |\
                                     PIN_OSPEED_2M(GPIOB_UART_TX) |\
                                     PIN_OSPEED_2M(11) |\
                                     PIN_OSPEED_2M(GPIOB_EN_GATE) |\
                                     PIN_OSPEED_2M(GPIOB_USB_VBUS_DETECT) |\
                                     PIN_OSPEED_2M(GPIOB_RCPWM) |\
                                     PIN_OSPEED_2M(GPIOB_CAN1_LED_INVERSE))

#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOB_MOTOR_PWM_2N) |\
                                     PIN_PUPDR_PULLDOWN(GPIOB_MOTOR_PWM_3N) |\
                                     PIN_PUPDR_PULLUP(GPIOB_GAIN) |\
                                     PIN_PUPDR_FLOATING(GPIOB_TEST_1) |\
                                     PIN_PUPDR_FLOATING(GPIOB_TEST_2) |\
                                     PIN_PUPDR_PULLUP(GPIOB_CAN2_RX) |\
                                     PIN_PUPDR_FLOATING(GPIOB_CAN2_TX) |\
                                     PIN_PUPDR_PULLUP(GPIOB_I2C_SDA) |\
                                     PIN_PUPDR_PULLUP(GPIOB_CAN1_RX) |\
                                     PIN_PUPDR_FLOATING(GPIOB_CAN1_TX) |\
                                     PIN_PUPDR_FLOATING(GPIOB_UART_TX) |\
                                     PIN_PUPDR_PULLDOWN(11) |\
                                     PIN_PUPDR_FLOATING(GPIOB_EN_GATE) |\
                                     PIN_PUPDR_PULLDOWN(GPIOB_USB_VBUS_DETECT) |\
                                     PIN_PUPDR_FLOATING(GPIOB_RCPWM) |\
                                     PIN_PUPDR_FLOATING(GPIOB_CAN1_LED_INVERSE))

#define VAL_GPIOB_ODR               (PIN_ODR_LOW(GPIOB_MOTOR_PWM_2N) |\
                                     PIN_ODR_LOW(GPIOB_MOTOR_PWM_3N) |\
                                     PIN_ODR_LOW(GPIOB_GAIN) |\
                                     PIN_ODR_LOW(GPIOB_TEST_1) |\
                                     PIN_ODR_LOW(GPIOB_TEST_2) |\
                                     PIN_ODR_HIGH(GPIOB_CAN2_RX) |\
                                     PIN_ODR_HIGH(GPIOB_CAN2_TX) |\
                                     PIN_ODR_HIGH(GPIOB_I2C_SDA) |\
                                     PIN_ODR_HIGH(GPIOB_CAN1_RX) |\
                                     PIN_ODR_HIGH(GPIOB_CAN1_TX) |\
                                     PIN_ODR_HIGH(GPIOB_UART_TX) |\
                                     PIN_ODR_LOW(11) |\
                                     PIN_ODR_LOW(GPIOB_EN_GATE) |\
                                     PIN_ODR_LOW(GPIOB_USB_VBUS_DETECT) |\
                                     PIN_ODR_LOW(GPIOB_RCPWM) |\
                                     PIN_ODR_HIGH(GPIOB_CAN1_LED_INVERSE))

#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_MOTOR_PWM_2N,  0) |\
                                     PIN_AFIO_AF(GPIOB_MOTOR_PWM_3N,  0) |\
                                     PIN_AFIO_AF(GPIOB_GAIN,  0) |\
                                     PIN_AFIO_AF(GPIOB_TEST_1,  0) |\
                                     PIN_AFIO_AF(GPIOB_TEST_2,  0) |\
                                     PIN_AFIO_AF(GPIOB_CAN2_RX,  9) |\
                                     PIN_AFIO_AF(GPIOB_CAN2_TX,  9) |\
                                     PIN_AFIO_AF(GPIOB_I2C_SDA,  0) )
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_CAN1_RX,  9) |\
                                     PIN_AFIO_AF(GPIOB_CAN1_TX,  9) |\
                                     PIN_AFIO_AF(GPIOB_UART_TX, 7) |\
                                     PIN_AFIO_AF(11, 0) |\
                                     PIN_AFIO_AF(GPIOB_EN_GATE, 0) |\
                                     PIN_AFIO_AF(GPIOB_USB_VBUS_DETECT, 0) |\
                                     PIN_AFIO_AF(GPIOB_RCPWM, 0) |\
                                     PIN_AFIO_AF(GPIOB_CAN1_LED_INVERSE, 0))

/*
 * GPIOC
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(GPIOC_TEMPERATURE_SENS) |\
                                     PIN_MODE_ANALOG(GPIOC_VBAT_SENS) |\
                                     PIN_MODE_ANALOG(GPIOC_CURRENT_2_SENS) |\
                                     PIN_MODE_ANALOG(GPIOC_CURRENT_1_SENS) |\
                                     PIN_MODE_ANALOG(GPIOC_MOTOR_TEMPERATURE_PULL_RES) |\
                                     PIN_MODE_ALTERNATE(GPIOC_UART_RX) |\
                                     PIN_MODE_INPUT(GPIOC_I2C_SCL) |\
                                     PIN_MODE_ALTERNATE(GPIOC_RGB_LED_RED_INVERSE) |\
                                     PIN_MODE_ALTERNATE(GPIOC_RGB_LED_GREEN_INVERSE) |\
                                     PIN_MODE_ALTERNATE(GPIOC_RGB_LED_BLUE_INVERSE) |\
                                     PIN_MODE_INPUT(GPIOC_HARDWARE_ID_BIT_0) |\
                                     PIN_MODE_INPUT(GPIOC_HARDWARE_ID_BIT_1) |\
                                     PIN_MODE_INPUT(GPIOC_HARDWARE_ID_BIT_2) |\
                                     PIN_MODE_INPUT(GPIOC_LVPS_MALFUNCTION_INVERSE) |\
                                     PIN_MODE_INPUT(GPIOC_VSI_OVERLOAD_INVERSE) |\
                                     PIN_MODE_INPUT(GPIOC_VSI_FAULT_INVERSE))

#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_TEMPERATURE_SENS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_VBAT_SENS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CURRENT_2_SENS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CURRENT_1_SENS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_MOTOR_TEMPERATURE_PULL_RES) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_UART_RX) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOC_I2C_SCL) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOC_RGB_LED_RED_INVERSE) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOC_RGB_LED_GREEN_INVERSE) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOC_RGB_LED_BLUE_INVERSE) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_HARDWARE_ID_BIT_0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_HARDWARE_ID_BIT_1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_HARDWARE_ID_BIT_2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_LVPS_MALFUNCTION_INVERSE) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_VSI_OVERLOAD_INVERSE) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_VSI_FAULT_INVERSE))

#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_2M(GPIOC_TEMPERATURE_SENS) |\
                                     PIN_OSPEED_2M(GPIOC_VBAT_SENS) |\
                                     PIN_OSPEED_2M(GPIOC_CURRENT_2_SENS) |\
                                     PIN_OSPEED_2M(GPIOC_CURRENT_1_SENS) |\
                                     PIN_OSPEED_2M(GPIOC_MOTOR_TEMPERATURE_PULL_RES) |\
                                     PIN_OSPEED_2M(GPIOC_UART_RX) |\
                                     PIN_OSPEED_2M(GPIOC_I2C_SCL) |\
                                     PIN_OSPEED_2M(GPIOC_RGB_LED_RED_INVERSE) |\
                                     PIN_OSPEED_2M(GPIOC_RGB_LED_GREEN_INVERSE) |\
                                     PIN_OSPEED_2M(GPIOC_RGB_LED_BLUE_INVERSE) |\
                                     PIN_OSPEED_2M(GPIOC_HARDWARE_ID_BIT_0) |\
                                     PIN_OSPEED_2M(GPIOC_HARDWARE_ID_BIT_1) |\
                                     PIN_OSPEED_2M(GPIOC_HARDWARE_ID_BIT_2) |\
                                     PIN_OSPEED_2M(GPIOC_LVPS_MALFUNCTION_INVERSE) |\
                                     PIN_OSPEED_2M(GPIOC_VSI_OVERLOAD_INVERSE) |\
                                     PIN_OSPEED_2M(GPIOC_VSI_FAULT_INVERSE))

#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_TEMPERATURE_SENS) |\
                                     PIN_PUPDR_FLOATING(GPIOC_VBAT_SENS) |\
                                     PIN_PUPDR_FLOATING(GPIOC_CURRENT_2_SENS) |\
                                     PIN_PUPDR_FLOATING(GPIOC_CURRENT_1_SENS) |\
                                     PIN_PUPDR_FLOATING(GPIOC_MOTOR_TEMPERATURE_PULL_RES) |\
                                     PIN_PUPDR_PULLUP(GPIOC_UART_RX) |\
                                     PIN_PUPDR_PULLUP(GPIOC_I2C_SCL) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RGB_LED_RED_INVERSE) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RGB_LED_GREEN_INVERSE) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RGB_LED_BLUE_INVERSE) |\
                                     PIN_PUPDR_PULLUP(GPIOC_HARDWARE_ID_BIT_0) |\
                                     PIN_PUPDR_FLOATING(GPIOC_HARDWARE_ID_BIT_1) |\
                                     PIN_PUPDR_FLOATING(GPIOC_HARDWARE_ID_BIT_2) |\
                                     PIN_PUPDR_PULLUP(GPIOC_LVPS_MALFUNCTION_INVERSE) |\
                                     PIN_PUPDR_PULLUP(GPIOC_VSI_OVERLOAD_INVERSE) |\
                                     PIN_PUPDR_PULLUP(GPIOC_VSI_FAULT_INVERSE))

#define VAL_GPIOC_ODR               (PIN_ODR_LOW(GPIOC_TEMPERATURE_SENS) |\
                                     PIN_ODR_LOW(GPIOC_VBAT_SENS) |\
                                     PIN_ODR_LOW(GPIOC_CURRENT_2_SENS) |\
                                     PIN_ODR_LOW(GPIOC_CURRENT_1_SENS) |\
                                     PIN_ODR_LOW(GPIOC_MOTOR_TEMPERATURE_PULL_RES) |\
                                     PIN_ODR_LOW(GPIOC_UART_RX) |\
                                     PIN_ODR_HIGH(GPIOC_I2C_SCL) |\
                                     PIN_ODR_HIGH(GPIOC_RGB_LED_RED_INVERSE) |\
                                     PIN_ODR_HIGH(GPIOC_RGB_LED_GREEN_INVERSE) |\
                                     PIN_ODR_HIGH(GPIOC_RGB_LED_BLUE_INVERSE) |\
                                     PIN_ODR_LOW(GPIOC_HARDWARE_ID_BIT_0) |\
                                     PIN_ODR_LOW(GPIOC_HARDWARE_ID_BIT_1) |\
                                     PIN_ODR_LOW(GPIOC_HARDWARE_ID_BIT_2) |\
                                     PIN_ODR_LOW(GPIOC_LVPS_MALFUNCTION_INVERSE) |\
                                     PIN_ODR_LOW(GPIOC_VSI_OVERLOAD_INVERSE) |\
                                     PIN_ODR_LOW(GPIOC_VSI_FAULT_INVERSE))

#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_TEMPERATURE_SENS,  0) |\
                                     PIN_AFIO_AF(GPIOC_VBAT_SENS,  0) |\
                                     PIN_AFIO_AF(GPIOC_CURRENT_2_SENS,  0) |\
                                     PIN_AFIO_AF(GPIOC_CURRENT_1_SENS,  0) |\
                                     PIN_AFIO_AF(GPIOC_MOTOR_TEMPERATURE_PULL_RES,  0) |\
                                     PIN_AFIO_AF(GPIOC_UART_RX,  7) |\
                                     PIN_AFIO_AF(GPIOC_I2C_SCL,  0) |\
                                     PIN_AFIO_AF(GPIOC_RGB_LED_RED_INVERSE,  2) )
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_RGB_LED_GREEN_INVERSE,  2) |\
                                     PIN_AFIO_AF(GPIOC_RGB_LED_BLUE_INVERSE,  2) |\
                                     PIN_AFIO_AF(GPIOC_HARDWARE_ID_BIT_0, 0) |\
                                     PIN_AFIO_AF(GPIOC_HARDWARE_ID_BIT_1, 0) |\
                                     PIN_AFIO_AF(GPIOC_HARDWARE_ID_BIT_2, 0) |\
                                     PIN_AFIO_AF(GPIOC_LVPS_MALFUNCTION_INVERSE, 0) |\
                                     PIN_AFIO_AF(GPIOC_VSI_OVERLOAD_INVERSE, 0) |\
                                     PIN_AFIO_AF(GPIOC_VSI_FAULT_INVERSE, 0))

/*
 * GPIOD
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(0) |\
                                     PIN_MODE_INPUT(1) |\
                                     PIN_MODE_OUTPUT(GPIOD_CAN_POWER_OUTPUT_ENABLE) |\
                                     PIN_MODE_INPUT(3) |\
                                     PIN_MODE_INPUT(4) |\
                                     PIN_MODE_INPUT(5) |\
                                     PIN_MODE_INPUT(6) |\
                                     PIN_MODE_INPUT(7) |\
                                     PIN_MODE_INPUT(8) |\
                                     PIN_MODE_INPUT(9) |\
                                     PIN_MODE_INPUT(10) |\
                                     PIN_MODE_INPUT(11) |\
                                     PIN_MODE_INPUT(12) |\
                                     PIN_MODE_INPUT(13) |\
                                     PIN_MODE_INPUT(14) |\
                                     PIN_MODE_INPUT(15))

#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(0) |\
                                     PIN_OTYPE_PUSHPULL(1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_CAN_POWER_OUTPUT_ENABLE) |\
                                     PIN_OTYPE_PUSHPULL(3) |\
                                     PIN_OTYPE_PUSHPULL(4) |\
                                     PIN_OTYPE_PUSHPULL(5) |\
                                     PIN_OTYPE_PUSHPULL(6) |\
                                     PIN_OTYPE_PUSHPULL(7) |\
                                     PIN_OTYPE_PUSHPULL(8) |\
                                     PIN_OTYPE_PUSHPULL(9) |\
                                     PIN_OTYPE_PUSHPULL(10) |\
                                     PIN_OTYPE_PUSHPULL(11) |\
                                     PIN_OTYPE_PUSHPULL(12) |\
                                     PIN_OTYPE_PUSHPULL(13) |\
                                     PIN_OTYPE_PUSHPULL(14) |\
                                     PIN_OTYPE_PUSHPULL(15))

#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_2M(0) |\
                                     PIN_OSPEED_2M(1) |\
                                     PIN_OSPEED_2M(GPIOD_CAN_POWER_OUTPUT_ENABLE) |\
                                     PIN_OSPEED_2M(3) |\
                                     PIN_OSPEED_2M(4) |\
                                     PIN_OSPEED_2M(5) |\
                                     PIN_OSPEED_2M(6) |\
                                     PIN_OSPEED_2M(7) |\
                                     PIN_OSPEED_2M(8) |\
                                     PIN_OSPEED_2M(9) |\
                                     PIN_OSPEED_2M(10) |\
                                     PIN_OSPEED_2M(11) |\
                                     PIN_OSPEED_2M(12) |\
                                     PIN_OSPEED_2M(13) |\
                                     PIN_OSPEED_2M(14) |\
                                     PIN_OSPEED_2M(15))

#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLDOWN(0) |\
                                     PIN_PUPDR_PULLDOWN(1) |\
                                     PIN_PUPDR_FLOATING(GPIOD_CAN_POWER_OUTPUT_ENABLE) |\
                                     PIN_PUPDR_PULLDOWN(3) |\
                                     PIN_PUPDR_PULLDOWN(4) |\
                                     PIN_PUPDR_PULLDOWN(5) |\
                                     PIN_PUPDR_PULLDOWN(6) |\
                                     PIN_PUPDR_PULLDOWN(7) |\
                                     PIN_PUPDR_PULLDOWN(8) |\
                                     PIN_PUPDR_PULLDOWN(9) |\
                                     PIN_PUPDR_PULLDOWN(10) |\
                                     PIN_PUPDR_PULLDOWN(11) |\
                                     PIN_PUPDR_PULLDOWN(12) |\
                                     PIN_PUPDR_PULLDOWN(13) |\
                                     PIN_PUPDR_PULLDOWN(14) |\
                                     PIN_PUPDR_PULLDOWN(15))

#define VAL_GPIOD_ODR               (PIN_ODR_LOW(0) |\
                                     PIN_ODR_LOW(1) |\
                                     PIN_ODR_LOW(GPIOD_CAN_POWER_OUTPUT_ENABLE) |\
                                     PIN_ODR_LOW(3) |\
                                     PIN_ODR_LOW(4) |\
                                     PIN_ODR_LOW(5) |\
                                     PIN_ODR_LOW(6) |\
                                     PIN_ODR_LOW(7) |\
                                     PIN_ODR_LOW(8) |\
                                     PIN_ODR_LOW(9) |\
                                     PIN_ODR_LOW(10) |\
                                     PIN_ODR_LOW(11) |\
                                     PIN_ODR_LOW(12) |\
                                     PIN_ODR_LOW(13) |\
                                     PIN_ODR_LOW(14) |\
                                     PIN_ODR_LOW(15))

#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(0,  0) |\
                                     PIN_AFIO_AF(1,  0) |\
                                     PIN_AFIO_AF(GPIOD_CAN_POWER_OUTPUT_ENABLE,  0) |\
                                     PIN_AFIO_AF(3,  0) |\
                                     PIN_AFIO_AF(4,  0) |\
                                     PIN_AFIO_AF(5,  0) |\
                                     PIN_AFIO_AF(6,  0) |\
                                     PIN_AFIO_AF(7,  0) )
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(8,  0) |\
                                     PIN_AFIO_AF(9,  0) |\
                                     PIN_AFIO_AF(10, 0) |\
                                     PIN_AFIO_AF(11, 0) |\
                                     PIN_AFIO_AF(12, 0) |\
                                     PIN_AFIO_AF(13, 0) |\
                                     PIN_AFIO_AF(14, 0) |\
                                     PIN_AFIO_AF(15, 0))

/*
 * GPIOE
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(0) |\
                                     PIN_MODE_INPUT(1) |\
                                     PIN_MODE_INPUT(2) |\
                                     PIN_MODE_INPUT(3) |\
                                     PIN_MODE_INPUT(4) |\
                                     PIN_MODE_INPUT(5) |\
                                     PIN_MODE_INPUT(6) |\
                                     PIN_MODE_INPUT(7) |\
                                     PIN_MODE_INPUT(8) |\
                                     PIN_MODE_INPUT(9) |\
                                     PIN_MODE_INPUT(10) |\
                                     PIN_MODE_INPUT(11) |\
                                     PIN_MODE_INPUT(12) |\
                                     PIN_MODE_INPUT(13) |\
                                     PIN_MODE_INPUT(14) |\
                                     PIN_MODE_INPUT(15))

#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(0) |\
                                     PIN_OTYPE_PUSHPULL(1) |\
                                     PIN_OTYPE_PUSHPULL(2) |\
                                     PIN_OTYPE_PUSHPULL(3) |\
                                     PIN_OTYPE_PUSHPULL(4) |\
                                     PIN_OTYPE_PUSHPULL(5) |\
                                     PIN_OTYPE_PUSHPULL(6) |\
                                     PIN_OTYPE_PUSHPULL(7) |\
                                     PIN_OTYPE_PUSHPULL(8) |\
                                     PIN_OTYPE_PUSHPULL(9) |\
                                     PIN_OTYPE_PUSHPULL(10) |\
                                     PIN_OTYPE_PUSHPULL(11) |\
                                     PIN_OTYPE_PUSHPULL(12) |\
                                     PIN_OTYPE_PUSHPULL(13) |\
                                     PIN_OTYPE_PUSHPULL(14) |\
                                     PIN_OTYPE_PUSHPULL(15))

#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_2M(0) |\
                                     PIN_OSPEED_2M(1) |\
                                     PIN_OSPEED_2M(2) |\
                                     PIN_OSPEED_2M(3) |\
                                     PIN_OSPEED_2M(4) |\
                                     PIN_OSPEED_2M(5) |\
                                     PIN_OSPEED_2M(6) |\
                                     PIN_OSPEED_2M(7) |\
                                     PIN_OSPEED_2M(8) |\
                                     PIN_OSPEED_2M(9) |\
                                     PIN_OSPEED_2M(10) |\
                                     PIN_OSPEED_2M(11) |\
                                     PIN_OSPEED_2M(12) |\
                                     PIN_OSPEED_2M(13) |\
                                     PIN_OSPEED_2M(14) |\
                                     PIN_OSPEED_2M(15))

#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLDOWN(0) |\
                                     PIN_PUPDR_PULLDOWN(1) |\
                                     PIN_PUPDR_PULLDOWN(2) |\
                                     PIN_PUPDR_PULLDOWN(3) |\
                                     PIN_PUPDR_PULLDOWN(4) |\
                                     PIN_PUPDR_PULLDOWN(5) |\
                                     PIN_PUPDR_PULLDOWN(6) |\
                                     PIN_PUPDR_PULLDOWN(7) |\
                                     PIN_PUPDR_PULLDOWN(8) |\
                                     PIN_PUPDR_PULLDOWN(9) |\
                                     PIN_PUPDR_PULLDOWN(10) |\
                                     PIN_PUPDR_PULLDOWN(11) |\
                                     PIN_PUPDR_PULLDOWN(12) |\
                                     PIN_PUPDR_PULLDOWN(13) |\
                                     PIN_PUPDR_PULLDOWN(14) |\
                                     PIN_PUPDR_PULLDOWN(15))

#define VAL_GPIOE_ODR               (PIN_ODR_LOW(0) |\
                                     PIN_ODR_LOW(1) |\
                                     PIN_ODR_LOW(2) |\
                                     PIN_ODR_LOW(3) |\
                                     PIN_ODR_LOW(4) |\
                                     PIN_ODR_LOW(5) |\
                                     PIN_ODR_LOW(6) |\
                                     PIN_ODR_LOW(7) |\
                                     PIN_ODR_LOW(8) |\
                                     PIN_ODR_LOW(9) |\
                                     PIN_ODR_LOW(10) |\
                                     PIN_ODR_LOW(11) |\
                                     PIN_ODR_LOW(12) |\
                                     PIN_ODR_LOW(13) |\
                                     PIN_ODR_LOW(14) |\
                                     PIN_ODR_LOW(15))

#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(0,  0) |\
                                     PIN_AFIO_AF(1,  0) |\
                                     PIN_AFIO_AF(2,  0) |\
                                     PIN_AFIO_AF(3,  0) |\
                                     PIN_AFIO_AF(4,  0) |\
                                     PIN_AFIO_AF(5,  0) |\
                                     PIN_AFIO_AF(6,  0) |\
                                     PIN_AFIO_AF(7,  0) )
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(8,  0) |\
                                     PIN_AFIO_AF(9,  0) |\
                                     PIN_AFIO_AF(10, 0) |\
                                     PIN_AFIO_AF(11, 0) |\
                                     PIN_AFIO_AF(12, 0) |\
                                     PIN_AFIO_AF(13, 0) |\
                                     PIN_AFIO_AF(14, 0) |\
                                     PIN_AFIO_AF(15, 0))

/*
 * GPIOF
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(0) |\
                                     PIN_MODE_INPUT(1) |\
                                     PIN_MODE_INPUT(2) |\
                                     PIN_MODE_INPUT(3) |\
                                     PIN_MODE_INPUT(4) |\
                                     PIN_MODE_INPUT(5) |\
                                     PIN_MODE_INPUT(6) |\
                                     PIN_MODE_INPUT(7) |\
                                     PIN_MODE_INPUT(8) |\
                                     PIN_MODE_INPUT(9) |\
                                     PIN_MODE_INPUT(10) |\
                                     PIN_MODE_INPUT(11) |\
                                     PIN_MODE_INPUT(12) |\
                                     PIN_MODE_INPUT(13) |\
                                     PIN_MODE_INPUT(14) |\
                                     PIN_MODE_INPUT(15))

#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(0) |\
                                     PIN_OTYPE_PUSHPULL(1) |\
                                     PIN_OTYPE_PUSHPULL(2) |\
                                     PIN_OTYPE_PUSHPULL(3) |\
                                     PIN_OTYPE_PUSHPULL(4) |\
                                     PIN_OTYPE_PUSHPULL(5) |\
                                     PIN_OTYPE_PUSHPULL(6) |\
                                     PIN_OTYPE_PUSHPULL(7) |\
                                     PIN_OTYPE_PUSHPULL(8) |\
                                     PIN_OTYPE_PUSHPULL(9) |\
                                     PIN_OTYPE_PUSHPULL(10) |\
                                     PIN_OTYPE_PUSHPULL(11) |\
                                     PIN_OTYPE_PUSHPULL(12) |\
                                     PIN_OTYPE_PUSHPULL(13) |\
                                     PIN_OTYPE_PUSHPULL(14) |\
                                     PIN_OTYPE_PUSHPULL(15))

#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_2M(0) |\
                                     PIN_OSPEED_2M(1) |\
                                     PIN_OSPEED_2M(2) |\
                                     PIN_OSPEED_2M(3) |\
                                     PIN_OSPEED_2M(4) |\
                                     PIN_OSPEED_2M(5) |\
                                     PIN_OSPEED_2M(6) |\
                                     PIN_OSPEED_2M(7) |\
                                     PIN_OSPEED_2M(8) |\
                                     PIN_OSPEED_2M(9) |\
                                     PIN_OSPEED_2M(10) |\
                                     PIN_OSPEED_2M(11) |\
                                     PIN_OSPEED_2M(12) |\
                                     PIN_OSPEED_2M(13) |\
                                     PIN_OSPEED_2M(14) |\
                                     PIN_OSPEED_2M(15))

#define VAL_GPIOF_PUPDR             (PIN_PUPDR_PULLDOWN(0) |\
                                     PIN_PUPDR_PULLDOWN(1) |\
                                     PIN_PUPDR_PULLDOWN(2) |\
                                     PIN_PUPDR_PULLDOWN(3) |\
                                     PIN_PUPDR_PULLDOWN(4) |\
                                     PIN_PUPDR_PULLDOWN(5) |\
                                     PIN_PUPDR_PULLDOWN(6) |\
                                     PIN_PUPDR_PULLDOWN(7) |\
                                     PIN_PUPDR_PULLDOWN(8) |\
                                     PIN_PUPDR_PULLDOWN(9) |\
                                     PIN_PUPDR_PULLDOWN(10) |\
                                     PIN_PUPDR_PULLDOWN(11) |\
                                     PIN_PUPDR_PULLDOWN(12) |\
                                     PIN_PUPDR_PULLDOWN(13) |\
                                     PIN_PUPDR_PULLDOWN(14) |\
                                     PIN_PUPDR_PULLDOWN(15))

#define VAL_GPIOF_ODR               (PIN_ODR_LOW(0) |\
                                     PIN_ODR_LOW(1) |\
                                     PIN_ODR_LOW(2) |\
                                     PIN_ODR_LOW(3) |\
                                     PIN_ODR_LOW(4) |\
                                     PIN_ODR_LOW(5) |\
                                     PIN_ODR_LOW(6) |\
                                     PIN_ODR_LOW(7) |\
                                     PIN_ODR_LOW(8) |\
                                     PIN_ODR_LOW(9) |\
                                     PIN_ODR_LOW(10) |\
                                     PIN_ODR_LOW(11) |\
                                     PIN_ODR_LOW(12) |\
                                     PIN_ODR_LOW(13) |\
                                     PIN_ODR_LOW(14) |\
                                     PIN_ODR_LOW(15))

#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(0,  0) |\
                                     PIN_AFIO_AF(1,  0) |\
                                     PIN_AFIO_AF(2,  0) |\
                                     PIN_AFIO_AF(3,  0) |\
                                     PIN_AFIO_AF(4,  0) |\
                                     PIN_AFIO_AF(5,  0) |\
                                     PIN_AFIO_AF(6,  0) |\
                                     PIN_AFIO_AF(7,  0) )
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(8,  0) |\
                                     PIN_AFIO_AF(9,  0) |\
                                     PIN_AFIO_AF(10, 0) |\
                                     PIN_AFIO_AF(11, 0) |\
                                     PIN_AFIO_AF(12, 0) |\
                                     PIN_AFIO_AF(13, 0) |\
                                     PIN_AFIO_AF(14, 0) |\
                                     PIN_AFIO_AF(15, 0))

/*
 * GPIOG
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(0) |\
                                     PIN_MODE_INPUT(1) |\
                                     PIN_MODE_INPUT(2) |\
                                     PIN_MODE_INPUT(3) |\
                                     PIN_MODE_INPUT(4) |\
                                     PIN_MODE_INPUT(5) |\
                                     PIN_MODE_INPUT(6) |\
                                     PIN_MODE_INPUT(7) |\
                                     PIN_MODE_INPUT(8) |\
                                     PIN_MODE_INPUT(9) |\
                                     PIN_MODE_INPUT(10) |\
                                     PIN_MODE_INPUT(11) |\
                                     PIN_MODE_INPUT(12) |\
                                     PIN_MODE_INPUT(13) |\
                                     PIN_MODE_INPUT(14) |\
                                     PIN_MODE_INPUT(15))

#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(0) |\
                                     PIN_OTYPE_PUSHPULL(1) |\
                                     PIN_OTYPE_PUSHPULL(2) |\
                                     PIN_OTYPE_PUSHPULL(3) |\
                                     PIN_OTYPE_PUSHPULL(4) |\
                                     PIN_OTYPE_PUSHPULL(5) |\
                                     PIN_OTYPE_PUSHPULL(6) |\
                                     PIN_OTYPE_PUSHPULL(7) |\
                                     PIN_OTYPE_PUSHPULL(8) |\
                                     PIN_OTYPE_PUSHPULL(9) |\
                                     PIN_OTYPE_PUSHPULL(10) |\
                                     PIN_OTYPE_PUSHPULL(11) |\
                                     PIN_OTYPE_PUSHPULL(12) |\
                                     PIN_OTYPE_PUSHPULL(13) |\
                                     PIN_OTYPE_PUSHPULL(14) |\
                                     PIN_OTYPE_PUSHPULL(15))

#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_2M(0) |\
                                     PIN_OSPEED_2M(1) |\
                                     PIN_OSPEED_2M(2) |\
                                     PIN_OSPEED_2M(3) |\
                                     PIN_OSPEED_2M(4) |\
                                     PIN_OSPEED_2M(5) |\
                                     PIN_OSPEED_2M(6) |\
                                     PIN_OSPEED_2M(7) |\
                                     PIN_OSPEED_2M(8) |\
                                     PIN_OSPEED_2M(9) |\
                                     PIN_OSPEED_2M(10) |\
                                     PIN_OSPEED_2M(11) |\
                                     PIN_OSPEED_2M(12) |\
                                     PIN_OSPEED_2M(13) |\
                                     PIN_OSPEED_2M(14) |\
                                     PIN_OSPEED_2M(15))

#define VAL_GPIOG_PUPDR             (PIN_PUPDR_PULLDOWN(0) |\
                                     PIN_PUPDR_PULLDOWN(1) |\
                                     PIN_PUPDR_PULLDOWN(2) |\
                                     PIN_PUPDR_PULLDOWN(3) |\
                                     PIN_PUPDR_PULLDOWN(4) |\
                                     PIN_PUPDR_PULLDOWN(5) |\
                                     PIN_PUPDR_PULLDOWN(6) |\
                                     PIN_PUPDR_PULLDOWN(7) |\
                                     PIN_PUPDR_PULLDOWN(8) |\
                                     PIN_PUPDR_PULLDOWN(9) |\
                                     PIN_PUPDR_PULLDOWN(10) |\
                                     PIN_PUPDR_PULLDOWN(11) |\
                                     PIN_PUPDR_PULLDOWN(12) |\
                                     PIN_PUPDR_PULLDOWN(13) |\
                                     PIN_PUPDR_PULLDOWN(14) |\
                                     PIN_PUPDR_PULLDOWN(15))

#define VAL_GPIOG_ODR               (PIN_ODR_LOW(0) |\
                                     PIN_ODR_LOW(1) |\
                                     PIN_ODR_LOW(2) |\
                                     PIN_ODR_LOW(3) |\
                                     PIN_ODR_LOW(4) |\
                                     PIN_ODR_LOW(5) |\
                                     PIN_ODR_LOW(6) |\
                                     PIN_ODR_LOW(7) |\
                                     PIN_ODR_LOW(8) |\
                                     PIN_ODR_LOW(9) |\
                                     PIN_ODR_LOW(10) |\
                                     PIN_ODR_LOW(11) |\
                                     PIN_ODR_LOW(12) |\
                                     PIN_ODR_LOW(13) |\
                                     PIN_ODR_LOW(14) |\
                                     PIN_ODR_LOW(15))

#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(0,  0) |\
                                     PIN_AFIO_AF(1,  0) |\
                                     PIN_AFIO_AF(2,  0) |\
                                     PIN_AFIO_AF(3,  0) |\
                                     PIN_AFIO_AF(4,  0) |\
                                     PIN_AFIO_AF(5,  0) |\
                                     PIN_AFIO_AF(6,  0) |\
                                     PIN_AFIO_AF(7,  0) )
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(8,  0) |\
                                     PIN_AFIO_AF(9,  0) |\
                                     PIN_AFIO_AF(10, 0) |\
                                     PIN_AFIO_AF(11, 0) |\
                                     PIN_AFIO_AF(12, 0) |\
                                     PIN_AFIO_AF(13, 0) |\
                                     PIN_AFIO_AF(14, 0) |\
                                     PIN_AFIO_AF(15, 0))

/*
 * GPIOH
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |\
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |\
                                     PIN_MODE_INPUT(2) |\
                                     PIN_MODE_INPUT(3) |\
                                     PIN_MODE_INPUT(4) |\
                                     PIN_MODE_INPUT(5) |\
                                     PIN_MODE_INPUT(6) |\
                                     PIN_MODE_INPUT(7) |\
                                     PIN_MODE_INPUT(8) |\
                                     PIN_MODE_INPUT(9) |\
                                     PIN_MODE_INPUT(10) |\
                                     PIN_MODE_INPUT(11) |\
                                     PIN_MODE_INPUT(12) |\
                                     PIN_MODE_INPUT(13) |\
                                     PIN_MODE_INPUT(14) |\
                                     PIN_MODE_INPUT(15))

#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |\
                                     PIN_OTYPE_PUSHPULL(2) |\
                                     PIN_OTYPE_PUSHPULL(3) |\
                                     PIN_OTYPE_PUSHPULL(4) |\
                                     PIN_OTYPE_PUSHPULL(5) |\
                                     PIN_OTYPE_PUSHPULL(6) |\
                                     PIN_OTYPE_PUSHPULL(7) |\
                                     PIN_OTYPE_PUSHPULL(8) |\
                                     PIN_OTYPE_PUSHPULL(9) |\
                                     PIN_OTYPE_PUSHPULL(10) |\
                                     PIN_OTYPE_PUSHPULL(11) |\
                                     PIN_OTYPE_PUSHPULL(12) |\
                                     PIN_OTYPE_PUSHPULL(13) |\
                                     PIN_OTYPE_PUSHPULL(14) |\
                                     PIN_OTYPE_PUSHPULL(15))

#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_2M(GPIOH_OSC_IN) |\
                                     PIN_OSPEED_2M(GPIOH_OSC_OUT) |\
                                     PIN_OSPEED_2M(2) |\
                                     PIN_OSPEED_2M(3) |\
                                     PIN_OSPEED_2M(4) |\
                                     PIN_OSPEED_2M(5) |\
                                     PIN_OSPEED_2M(6) |\
                                     PIN_OSPEED_2M(7) |\
                                     PIN_OSPEED_2M(8) |\
                                     PIN_OSPEED_2M(9) |\
                                     PIN_OSPEED_2M(10) |\
                                     PIN_OSPEED_2M(11) |\
                                     PIN_OSPEED_2M(12) |\
                                     PIN_OSPEED_2M(13) |\
                                     PIN_OSPEED_2M(14) |\
                                     PIN_OSPEED_2M(15))

#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |\
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |\
                                     PIN_PUPDR_PULLDOWN(2) |\
                                     PIN_PUPDR_PULLDOWN(3) |\
                                     PIN_PUPDR_PULLDOWN(4) |\
                                     PIN_PUPDR_PULLDOWN(5) |\
                                     PIN_PUPDR_PULLDOWN(6) |\
                                     PIN_PUPDR_PULLDOWN(7) |\
                                     PIN_PUPDR_PULLDOWN(8) |\
                                     PIN_PUPDR_PULLDOWN(9) |\
                                     PIN_PUPDR_PULLDOWN(10) |\
                                     PIN_PUPDR_PULLDOWN(11) |\
                                     PIN_PUPDR_PULLDOWN(12) |\
                                     PIN_PUPDR_PULLDOWN(13) |\
                                     PIN_PUPDR_PULLDOWN(14) |\
                                     PIN_PUPDR_PULLDOWN(15))

#define VAL_GPIOH_ODR               (PIN_ODR_LOW(GPIOH_OSC_IN) |\
                                     PIN_ODR_LOW(GPIOH_OSC_OUT) |\
                                     PIN_ODR_LOW(2) |\
                                     PIN_ODR_LOW(3) |\
                                     PIN_ODR_LOW(4) |\
                                     PIN_ODR_LOW(5) |\
                                     PIN_ODR_LOW(6) |\
                                     PIN_ODR_LOW(7) |\
                                     PIN_ODR_LOW(8) |\
                                     PIN_ODR_LOW(9) |\
                                     PIN_ODR_LOW(10) |\
                                     PIN_ODR_LOW(11) |\
                                     PIN_ODR_LOW(12) |\
                                     PIN_ODR_LOW(13) |\
                                     PIN_ODR_LOW(14) |\
                                     PIN_ODR_LOW(15))

#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN,  0) |\
                                     PIN_AFIO_AF(GPIOH_OSC_OUT,  0) |\
                                     PIN_AFIO_AF(2,  0) |\
                                     PIN_AFIO_AF(3,  0) |\
                                     PIN_AFIO_AF(4,  0) |\
                                     PIN_AFIO_AF(5,  0) |\
                                     PIN_AFIO_AF(6,  0) |\
                                     PIN_AFIO_AF(7,  0) )
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(8,  0) |\
                                     PIN_AFIO_AF(9,  0) |\
                                     PIN_AFIO_AF(10, 0) |\
                                     PIN_AFIO_AF(11, 0) |\
                                     PIN_AFIO_AF(12, 0) |\
                                     PIN_AFIO_AF(13, 0) |\
                                     PIN_AFIO_AF(14, 0) |\
                                     PIN_AFIO_AF(15, 0))


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
    void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */
