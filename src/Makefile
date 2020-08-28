ifneq ($(V), 1)
MAKEFLAGS += --no-print-dir
Q := @
endif

# Project.
BASE := ..

# Run mkdir if directory does not exists.
# $(call mkdir_if_needed,FILE)
mkdir_if_needed = $(if $(wildcard $(dir $1)),,$(Q)$(MKDIR) $(dir $1))

# Directory commands.
RM := rm -f
RMDIR := rm -rf
MKDIR := mkdir -p

# Objects directory.
OBJDIR := obj
VERSION := $(OBJDIR)/version.h

# Build programs.
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy

# User overwrittable directives.
OPT ?= -Og
DEBUG ?= -g3
SPECS ?= nano.specs
CROSS_COMPILE ?= arm-none-eabi-
LIB_OPENCM3_DIR ?= $(BASE)/libopencm3

# Program name and sources.
PROGRAM = program
SRCS += main.c
SRCS += cdcacm.c
SRCS += common.c
SRCS += platform.c
SRCS += syscalls.c

# Build and link flags.
ARCH = -mthumb -mcpu=cortex-m3
DEPS = -MP -MMD
DEFS = -DSTM32F1
INCS = -I. -I$(OBJDIR) -I$(LIB_OPENCM3_DIR)/include

CFLAGS += $(OPT) $(DEBUG) $(DEPS)
CFLAGS += $(ARCH) $(DEFS) $(INCS)
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-function
CFLAGS += -fno-common
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections

LDPATHS += -L$(BASE)/ld
LDPATHS += -L$(LIB_OPENCM3_DIR)/lib

LDFLAGS += $(ARCH) $(LDPATHS)
LDFLAGS += -specs=$(SPECS) -nostartfiles
LDFLAGS += -Wl,-gc-sections
LDFLAGS += -Wl,--build-id
LDFLAGS += -Wl,-Tbluepill.ld
LDFLAGS += -Wl,-Map=$(PROGRAM).map -Wl,--cref

LDLIBS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
LDLIBS += -lopencm3_stm32f1

# Object files.
.SECONDARY: $(OBJS)
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

# Make targets.
.PHONY: all
all: $(PROGRAM).elf

.PHONY: clean
clean:
	@echo "RMDIR   $(OBJDIR)"
	$(Q)$(RMDIR) $(OBJDIR)
	@echo "RM      $(PROGRAM).{bin,hex,elf.map}"
	$(Q)$(RM) $(PROGRAM).bin $(PROGRAM).hex $(PROGRAM).elf $(PROGRAM).map

GITDESC := $(shell git describe --always --dirty)
GENVERS := $(shell grep -sv $(GITDESC) $(VERSION))
.PHONY: $(if $(GENVERS),$(VERSION))
$(VERSION):
	@echo "GIT     $@"
	$(Q)echo "#define FIRMWARE_VERSION \"$(GITDESC)\"" > $@

$(OBJDIR):
	@echo "MKDIR   $@"
	$(Q)$(MKDIR) $@

# Binary rules.
%.elf: $(OBJS)
	@echo "LD      $@"
	$(Q)$(CC) $(OBJS) $(LDFLAGS) -o $@ $(LDLIBS)

%.bin: %.elf
	@echo "OBJCOPY $@"
	$(Q)$(OBJCOPY) -O binary $^ $@

%.hex: %.elf
	@echo "OBJCOPY $@"
	$(Q)$(OBJCOPY) -O ihex $^ $@

# Object rules.
$(OBJDIR)/%.o: %.c | $(OBJDIR) $(OBJDIR)/version.h
	@echo "CC      $<"
	$(call mkdir_if_needed,$@)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# Define a helper macro for debugging make errors online you
# can type "make print-CFLAGS" and it will show you how that
# ended up being resolved by all of the included makefiles.
print-%:
	@echo $*=$($*)

# Include generated dependency rules.
-include $(OBJS:.o=.d)
