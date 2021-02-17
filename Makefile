#
# Copyright (c) 2016-2018  Zubax Robotics  <info@zubax.com>
#
# Author: Pavel Kirienko <pavel.kirienko@zubax.com>
#

PROJECT = kocherga_demo

HW_VERSION_MAJOR = 1
BL_VERSION_MAJOR = 0
BL_VERSION_MINOR = 1

APPLICATION_OFFSET = 49152

#
# Bootloader
#

CPPSRC = $(shell find src -name '*.cpp' | sort)
CSRC   = $(shell find src -name '*.c'   | sort)

UINCDIR = src                \
          src/os

UDEFS = -DBL_VERSION_MAJOR=$(BL_VERSION_MAJOR)           \
        -DBL_VERSION_MINOR=$(BL_VERSION_MINOR)           \
        -DHW_VERSION_MAJOR=$(HW_VERSION_MAJOR)           \
        -DAPPLICATION_OFFSET=$(APPLICATION_OFFSET)       \
        -DPRODUCT_ID_STRING=\"$(PROJECT)\"               \
        -DPRODUCT_NAME_STRING=\"Kocherga\ Demo\"

#
# Libcanard library
#

UINCDIR += libcanard                                     \
           libcanard/drivers/stm32

CSRC += libcanard/canard.c                               \
        libcanard/drivers/stm32/canard_stm32.c

#
# Senoval library
#

UINCDIR += senoval

#
# Popcop library
#

UINCDIR += popcop/c++

#
# Kocherga library
#

UINCDIR += kocherga

#
# ChibiOS
#

CHIBIOS = chibios

USE_FPU = hard
MCU = cortex-m4

USE_PROCESS_STACKSIZE = 0x1000
USE_EXCEPTIONS_STACKSIZE = 0x1000

DDEFS += -DCRT1_AREAS_NUMBER=0

USE_OPT += -Wdouble-promotion -Wswitch-enum -Wfloat-equal -fno-strict-aliasing -fno-strict-overflow	\
           -Wno-implicit-fallthrough
USE_CPPOPT += -Wconversion -Wno-error=conversion -Wsign-promo -Wframe-larger-than=1024 -Werror=implicit-fallthrough

USE_LTO := yes

LDSCRIPT = ld.ld

UDEFS += -DCORTEX_ENABLE_WFI_IDLE=1

USE_LINK_GC         = yes
USE_THUMB           = yes
USE_VERBOSE_COMPILE = yes
USE_SMART_BUILD     = no

# Generic OS includes
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk
include $(CHIBIOS)/os/hal/lib/streams/streams.mk

# MCU-specific OS includes
#L4 -> Nucleo
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f4xx.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk

VARIOUSSRC = $(STREAMSSRC) \
             $(CHIBIOS)/os/various/syscalls.c

VARIOUSINC = $(STREAMSINC)

CSRC += $(STARTUPSRC) $(KERNSRC) $(PORTSRC) $(OSALSRC) $(HALSRC) $(PLATFORMSRC) $(VARIOUSSRC)

CPPSRC += $(CHCPPSRC)

ASMXSRC += $(STARTUPASM) $(PORTASM) $(OSALASM)

INCDIR += $(PORTINC) $(KERNINC) $(HALINC) $(PLATFORMINC) $(CHCPPINC) $(STARTUPINC) $(OSALINC) $(VARIOUSINC) \
          $(CHIBIOS)/os/various \
          $(CHIBIOS)/os/license


NO_BUILTIN += -fno-builtin-printf -fno-builtin-fprintf  -fno-builtin-vprintf -fno-builtin-vfprintf -fno-builtin-puts

USE_OPT += -falign-functions=16 -U__STRICT_ANSI__ -fno-exceptions -fno-unwind-tables -fno-stack-protector \
           $(NO_BUILTIN)

# Explicit usage flags are needed for LTO:
USE_OPT += -u_port_lock -u_port_unlock -u_exit -u_kill -u_getpid -uchThdExit -u__errno

# Fixing float constants - otherwise the C++ standard library may fail to compile.
# See http://www.chibios.com/forum/viewtopic.php?f=35&t=4619
# Adding this flag to several variables for extra paranoia; should be removed when ChibiOS is fixed
USE_OPT += -fno-single-precision-constant
UDEFS += -fno-single-precision-constant

# Unfortunately, we have to allow the use of the deprecated "register" keyword, because it is used in the
# sources of the operating system.
USE_COPT += -std=c99
USE_CPPOPT += -std=c++17 -fno-rtti -fno-exceptions -fno-threadsafe-statics -Wno-error=register -Wno-register

USE_OPT += -nodefaultlibs -lc -lgcc -lm

# https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/Optimize-Options.html
USE_OPT += -Os

RELEASE ?= 0
ifneq ($(RELEASE),0)
    DDEFS   += -DRELEASE_BUILD=1 -DNDEBUG=1
    DADEFS  += -DRELEASE_BUILD=1 -DNDEBUG=1
    USE_OPT += -fomit-frame-pointer -DNDEBUG
else
    DDEFS   += -DDEBUG_BUILD=1
    DADEFS  += -DDEBUG_BUILD=1
    USE_OPT += -g3
endif

TOOLCHAIN_PREFIX ?= arm-none-eabi
CC   = $(TOOLCHAIN_PREFIX)-gcc
CPPC = $(TOOLCHAIN_PREFIX)-g++
LD   = $(TOOLCHAIN_PREFIX)-g++
CP   = $(TOOLCHAIN_PREFIX)-objcopy
AS   = $(TOOLCHAIN_PREFIX)-gcc -x assembler-with-cpp
OD   = $(TOOLCHAIN_PREFIX)-objdump
SZ   = $(TOOLCHAIN_PREFIX)-size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# THUMB-specific options here
TOPT = -mthumb -DTHUMB=1

CWARN += -Wall -Wextra -Wstrict-prototypes
CPPWARN += -Wundef -Wall -Wextra -Werror

# asm statement fix
DDEFS += -Dasm=__asm

# This rule hook is defined in the ChibiOS build system
POST_MAKE_ALL_RULE_HOOK: build/$(PROJECT).bin build/$(PROJECT).elf
	cd build && mv -f $(PROJECT).bin bootloader.bin
	cd build && mv -f $(PROJECT).elf bootloader.elf

upload: build/bootloader.elf
	./upload_via_black_magic_probe.sh

include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/rules.mk
