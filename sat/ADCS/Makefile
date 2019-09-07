
SATDIR ?= $(TOPDIR)/sat
ARCH   ?= stm32f4

OBJFILES = \
   $(patsubst %c,%o,$(wildcard ./*/*.c)) \
   $(patsubst %cpp,%o,$(wildcard ./*/*.cpp)) \
   $(patsubst %c,%o,$(wildcard ./*.c)) \
   $(patsubst %cpp,%o,$(wildcard ./*.cpp)) \
   $(patsubst %c,%o,$(wildcard ./lib/armadillo/src/*.c)) \
   $(patsubst %cpp,%o,$(wildcard ./lib/armadillo/src/*.cpp))

TARGETS = ADCS.hex ADCS.bin ADCS.elf

CFLAGS += -I. $(APP_CFLAGS)
CFLAGS += -I./lib/armadillo/include
CFLAGS += -fdata-sections -ffunction-sections

LDFLAGS += -T$(SATDIR)/rules/arch-$(ARCH).ld
LDFLAGS += -Wl,-gc-sections
#LDFLAGS += -Wl,-Map=ADCS.map
LDFLAGS += -specs=nosys.specs
LDFLAGS += -L$(SATDIR)/sys/ -lsystem

default: $(TARGETS)

ADCS.hex: ADCS.elf
	@echo " [HEX] $@"
	$(QUIET) $(CP) -O ihex $< $@

ADCS.bin: ADCS.elf
	@echo " [BIN] $@"
	$(QUIET) $(CP) -O binary $< $@

ADCS.elf: $(OBJFILES)
	@echo " [ELF] $@"
	$(QUIET) $(CXX) $(MCUFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(TARGETS) $(OBJFILES)

flash: ADCS.bin
	$(FLASH) write $< 0x08000000

.PHONY: default clean flash

include $(SATDIR)/rules/common.mk

