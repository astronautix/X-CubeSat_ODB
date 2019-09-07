
SATDIR ?= $(TOPDIR)/sat

# this file is included by rules/common.mk when ARCH is set to stm32f4

AR  := arm-none-eabi-ar rcs
AS  := arm-none-eabi-as
CC  := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
CP  := arm-none-eabi-objcopy

# see https://launchpad.net/~germia/+archive/ubuntu/archive3
MCUFLAGS += -mthumb -mcpu=cortex-m4 -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb-interwork

#CFLAGS += -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_FULL_ASSERT
CFLAGS += -DSTM32F4XX -DUSE_STDPERIPH_DRIVER

ARCH_CFLAGS  = -I$(SATDIR)/sys/$(ARCH)/inc
ARCH_CFLAGS += -I$(SATDIR)/sys/$(ARCH)/inc/core
ARCH_CFLAGS += -I$(SATDIR)/sys/$(ARCH)/inc/peripherals

