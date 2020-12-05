
# .gdb-startup assumes the elf is here
BUILDDIR = build
TARGET = $(BUILDDIR)/main.elf

BOARD ?= samd10

ENABLE_SEMIHOSTING ?= 1

ifeq (samd10,$(BOARD))
# TODO confirm this layout is right
LDSCRIPT ?= devices/samd10d13am_flash.ld
ARCHFLAGS ?= -mcpu=cortex-m0plus
OPENOCD_CFG ?=
endif

CFLAGS += $(ARCHFLAGS)

CFLAGS += -ffunction-sections -fdata-sections

CFLAGS += -g3 -Os

CFLAGS += -Wall -Werror

CFLAGS += -mlittle-endian -mthumb -mthumb-interwork

ifeq (1,$(ENABLE_SEMIHOSTING))
LDFLAGS += --specs=rdimon.specs -lc -lrdimon
CFLAGS += -DENABLE_SEMIHOSTING=1
else
LDFLAGS += --specs=nano.specs
CFLAGS += -DENABLE_SEMIHOSTING=0
endif

LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections,-Map,$(TARGET).map

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
SIZE = arm-none-eabi-size
RM = rm -rf

SRCS = \
    main.c \
    startup.c \

OBJS = $(patsubst %.c, %.o, $(SRCS))
OBJS := $(addprefix $(BUILDDIR)/,$(OBJS))

# VPATH = ./

all: $(TARGET)

$(BUILDDIR):
	mkdir -p $@

clean:
	$(RM) $(BUILDDIR)

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	$(SIZE) $(TARGET)

openocd: build
	openocd -f $(OPENOCD_CFG)

gdb: $(TARGET)
	arm-none-eabi-gdb-py $(TARGET) -ex "source .gdb-startup"

.PHONY: all clean openocd gdb
