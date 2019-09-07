
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

