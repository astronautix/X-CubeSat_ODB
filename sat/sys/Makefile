
ARCHIVE = libsystem.a

OBJ_ARCH = \
   $(patsubst %s,%o,$(wildcard ./$(ARCH)/src/*.s)) \
   $(patsubst %c,%o,$(wildcard ./$(ARCH)/src/*.c)) \
   $(patsubst %c,%o,$(wildcard ./$(ARCH)/src/peripherals/*.c))

OBJ_RTOS = \
   $(patsubst %c,%o,$(wildcard ./FreeRTOS/*.c)) \
   $(patsubst %c,%o,$(wildcard ./FreeRTOS/portable/MemMang/heap_4.c)) \
   $(patsubst %c,%o,$(wildcard ./FreeRTOS/portable/$(ARCH)/*.c)) \
   $(patsubst %c,%o,$(wildcard ./sys/*.c)) \
   $(patsubst %cpp,%o,$(wildcard ./sys/*.cpp))

OBJ_DEVICE = \
   $(patsubst %cpp,%o,$(wildcard ./device/*.cpp)) \
   $(patsubst %cpp,%o,$(wildcard ./device/*/*.cpp))

OBJ_SYSTEM = \
   $(patsubst %c,%o,$(wildcard ./system/*.c)) \
   $(patsubst %cpp,%o,$(wildcard ./system/*.cpp))

OBJFILES = $(OBJ_ARCH) $(OBJ_RTOS) $(OBJ_DEVICE) $(OBJ_SYSTEM)

$(OBJ_ARCH):   CFLAGS += $(ARCH_CFLAGS)
$(OBJ_RTOS):   CFLAGS += -I./inc $(RTOS_CFLAGS)
$(OBJ_DEVICE): CFLAGS += -I. -I./device -I$(SATDIR) $(ARCH_CFLAGS) $(RTOS_CFLAGS)
$(OBJ_SYSTEM): CFLAGS += -I. -I./device -I$(SATDIR) $(ARCH_CFLAGS) $(RTOS_CFLAGS)

default: $(ARCHIVE)

clean:
	$(RM) $(ARCHIVE) $(OBJFILES)

include $(SATDIR)/rules/common.mk

