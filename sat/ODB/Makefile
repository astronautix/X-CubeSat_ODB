# @Author: Olivier Piras
# @Date:   2020-06-02T16:07:30+02:00
# @Email:  qb50@oprs.eu
# @Project: X-CubeSat
# @Last modified by:   Olivier Piras
# @Last modified time: 2020-06-02T16:21:24+02:00
# @License: This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
# @Copyright: Copyright (C) 2027 Olivier Piras  



SATDIR ?= $(TOPDIR)/sat
ARCH   ?= stm32f4

OBJFILES = \
   $(patsubst %c,%o,$(wildcard ./*/*.c)) \
   $(patsubst %cpp,%o,$(wildcard ./*/*.cpp)) \
   $(patsubst %c,%o,$(wildcard ./*.c)) \
   $(patsubst %cpp,%o,$(wildcard ./*.cpp))

OBJFILES := $(patsubst %cpp,%o,$(OBJFILES))

TARGETS = ODB.hex ODB.bin ODB.elf

CFLAGS += -I. $(APP_CFLAGS)
CFLAGS += -fdata-sections -ffunction-sections

LDFLAGS += -T$(SATDIR)/rules/arch-$(ARCH).ld
LDFLAGS += -Wl,-gc-sections
#LDFLAGS += -Wl,-Map=ODB.map
LDFLAGS += -specs=nosys.specs
LDFLAGS += -L$(SATDIR)/sys/ -lsystem

default: $(TARGETS)

ODB.hex: ODB.elf
	@echo " [HEX] $@"
	$(QUIET) $(CP) -O ihex $< $@

ODB.bin: ODB.elf
	@echo " [BIN] $@"
	$(QUIET) $(CP) -O binary $< $@

ODB.elf: $(OBJFILES)
	@echo " [ELF] $@"
	$(QUIET) $(CXX) $(MCUFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(TARGETS) $(OBJFILES)

flash: ODB.bin
	$(FLASH) write $< 0x08000000

.PHONY: default clean flash

include $(SATDIR)/rules/common.mk

