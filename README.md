Kocherga demo
=============

An ARM GCC toolchain v7.3 is needed.

## Building

In order to build the bootloader, execute `make RELEASE=1`.
Omit setting the `RELEASE` variable to build the debug version (it won't fit into the bootloader area).

Once the release version of the bootloader is built, its binary (`.bin`)
will have to be manually copied into the firmware source root directory.
This is arranged this way to ensure that once a working version of bootloader is finished and tested,
it will stay frozen in this exact configuration until updated explicitly.
The bootloader has to be robust and is not expected to change frequently.

## MCU Usage

### Timers

The following list documents the current usage of hardware timers.
All timers are clocked at 180 MHz.
Note that ChibiOS 16.1 does not recognize that; please read this post for details:
<http://www.chibios.com/forum/viewtopic.php?f=35&t=3870>.

Timer   | Resolution| Usage
--------|-----------|--------------------------------------------------------------------------------------------------
TIM1    | 16        | *Not used*
TIM2    | **32**    | RTOS System Timer (tickless)
TIM3    | 16        | RGB LED PWM
TIM4    | 16        | *Not used*
TIM5    | **32**    | *Not used*
TIM6    | 16        | *Not used*
TIM7    | 16        | *Not used*
TIM8    | 16        | *Not used*
TIM9    | 16        | *Not used*
TIM10   | 16        | *Not used*
TIM11   | 16        | *Not used*
TIM12   | 16        | *Not used*
TIM13   | 16        | *Not used*
TIM14   | 16        | *Not used*

### Watchdog

Release builds of the bootloader initialize the watchdog with a large timeout.
This is needed to prevent broken firmwares from bricking the device.

Debug builds to not initialize the watchdog in order to not interfere with debugging.

### RCC

The RCC CSR register contains reset cause flags.
If either of the watchdog timers is known to have caused the last reset,
the bootloader will inject a 10 second timeout before booting the application,
allowing external systems or the user to intervene and replace the bad firmware.
The reset cause flags are cleared by the bootloader.
