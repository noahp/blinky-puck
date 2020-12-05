
# .gdb-startup assumes the elf is here
BUILDDIR = build
TARGET = $(BUILDDIR)/main.elf

BOARD ?= samd10

# too big, need the poor man's version...
ENABLE_SEMIHOSTING ?= 0

ifeq (samd10,$(BOARD))
# TODO confirm this layout is right
LDSCRIPT ?= devices/samd10d13am_flash.ld
ARCHFLAGS ?= -mcpu=cortex-m0plus -D__SAMD10D13AM__
OPENOCD_CFG ?=
endif

CFLAGS += $(ARCHFLAGS)

CFLAGS += -ffunction-sections -fdata-sections

CFLAGS += -ggdb3

ifeq (1,$(DEBUG))
CFLAGS += -Og
else
CFLAGS += -Os
endif

CFLAGS += -Wall -Werror -Wextra
CFLAGS += \
    -Werror=conversion \
    -Werror=sign-conversion \
    -Werror=undef \
    -Werror=shadow \
    -Werror=double-promotion \
    -Werror=jump-misses-init \
    -Werror=logical-op \
    -Werror=format=2 \
    -Werror=duplicated-cond \
    -Werror=duplicated-branches \
    -Werror=implicit-fallthrough \
    -Werror=null-dereference \

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
    lib/samd10/source/gcc/startup_samd10.c \

INCS = \
    . \
    lib/cmsis \
    lib/samd10/include \
    lib/samd10/source \

CFLAGS += $(addprefix -I,$(INCS))

OBJS = $(patsubst %.c, %.o, $(SRCS))
OBJS := $(addprefix $(BUILDDIR)/,$(OBJS))

all: $(TARGET)

.SILENT:

$(BUILDDIR):
	mkdir -p $@

clean:
	$(RM) $(BUILDDIR)

# these are the compiler flags for emitting the dependency tracking
# file. Note the usage of the '$<' automatic variable
DEPFLAGS = -MMD -MP -MF $@.d

# bring in the prerequisites by including all the .d files. prefix
# the line with '-' to prevent an error if any of the files do not
# exist
-include $(wildcard $(BUILDDIR)/*.d)

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	mkdir -p $(dir $@)
	echo "CC $(notdir $<)"
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(LDSCRIPT)
	echo "Linking $(notdir $@)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $(TARGET)

openocd: build
	openocd -f $(OPENOCD_CFG)

gdb: $(TARGET)
	arm-none-eabi-gdb-py $(TARGET) -ex "source .gdb-startup"

.PHONY: all clean openocd gdb
