# CasualSBI - Hardware Abstraction Layer for ARM and RISC-V
# Build System

# Target configuration
ARCH        ?= riscv
SUBARCH     ?= rv64

# Architecture-specific defaults
ifeq ($(ARCH),riscv)
CROSS_COMPILE ?= riscv64-unknown-elf-
PLATFORM    ?= qemu-virt-riscv64
else ifeq ($(ARCH),arm)
CROSS_COMPILE ?= aarch64-linux-gnu-
SUBARCH     = rv64
PLATFORM    ?= qemu-virt-aarch64
endif

# Tools
CC          = $(CROSS_COMPILE)gcc
LD          = $(CROSS_COMPILE)ld
OBJCOPY     = $(CROSS_COMPILE)objcopy
OBJDUMP     = $(CROSS_COMPILE)objdump
SIZE        = $(CROSS_COMPILE)size

# Directories
BUILD_DIR   = build/$(ARCH)-$(SUBARCH)-$(PLATFORM)
SRC_DIR     = .
ARCH_DIR    = arch/$(ARCH)/$(SUBARCH)
PLAT_DIR    = platform/$(PLATFORM)

# Source files
C_SOURCES   = $(wildcard $(SRC_DIR)/lib/*.c) \
              $(wildcard $(SRC_DIR)/$(ARCH_DIR)/*.c) \
              $(wildcard $(SRC_DIR)/$(PLAT_DIR)/*.c)

-include $(SRC_DIR)/$(PLAT_DIR)/defconfig
# Source drivers
ifeq ($(CONFIG_DRIVER_UART_PL011C), y)
C_SOURCES	+= $(SRC_DIR)/drivers/uart/uart-pl011.c
endif
ifeq ($(CONFIG_DRIVER_UART_VIRT), y)
C_SOURCES	+= $(SRC_DIR)/drivers/uart/uart-virt.c
endif
ifeq ($(CONFIG_DRIVER_PLIC), y)
C_SOURCES	+= $(SRC_DIR)/drivers/plic/plic.c
endif

ASM_SOURCES = $(wildcard $(SRC_DIR)/$(ARCH_DIR)/*.S)

OBJECTS     = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(C_SOURCES))) \
              $(patsubst %.S,$(BUILD_DIR)/%.o,$(notdir $(ASM_SOURCES)))

VPATH       = $(SRC_DIR)/lib $(SRC_DIR)/$(ARCH_DIR) $(SRC_DIR)/$(PLAT_DIR) $(SRC_DIR)/drivers/uart $(SRC_DIR)/drivers/plic

# Architecture-specific compiler flags
ifeq ($(ARCH),riscv)
ARCH_CFLAGS = -march=rv64imafdc -mabi=lp64d -mcmodel=medany
ARCH_LD     = riscv
else ifeq ($(ARCH),arm)
ARCH_CFLAGS = -march=armv8-a -mgeneral-regs-only
ARCH_LD     = arm
endif

# Common compiler flags
CFLAGS      = $(ARCH_CFLAGS)
CFLAGS     += -Wall -Wextra -O2 -g -nostdlib -nostartfiles -ffreestanding
CFLAGS     += -I$(SRC_DIR)/include -I$(SRC_DIR)/include/arch/$(ARCH) -I$(SRC_DIR)
CFLAGS     += -DARCH_$(subst -,_,$(shell echo $(ARCH) | tr a-z A-Z))
CFLAGS     += -DPLATFORM_$(subst -,_,$(shell echo $(PLATFORM) | tr a-z A-Z))

# Linker flags
LDFLAGS     = -T $(SRC_DIR)/$(PLAT_DIR)/linker.ld -nostdlib

# Target
TARGET      = $(BUILD_DIR)/casualsbi.elf
TARGET_BIN  = $(BUILD_DIR)/casualsbi.bin

# Default target
.PHONY: all clean run debug

all: $(TARGET) $(TARGET_BIN)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $@

# Compile C files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly files
$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	@echo "AS  $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link
LINKER_SCRIPT = $(SRC_DIR)/$(PLAT_DIR)/linker.ld

$(TARGET): $(OBJECTS) $(LINKER_SCRIPT)
	@echo "LD  $@"
	@$(LD) $(LDFLAGS) $(OBJECTS) -o $@
	@echo "SIZE:"
	@$(SIZE) $@

# Binary
$(TARGET_BIN): $(TARGET)
	@echo "OBJCOPY $@"
	@$(OBJCOPY) -O binary $< $@

# QEMU run
ifeq ($(ARCH),riscv)
QEMU_ARCH = riscv64
QEMU_MACHINE = virt
QEMU_CPU = rv64
QEMU_BIOS = -bios none
else ifeq ($(ARCH),arm)
QEMU_ARCH = aarch64
QEMU_MACHINE = virt
QEMU_CPU = cortex-a72
QEMU_BIOS =
endif

run: $(TARGET)
	@echo "Running on QEMU ($(QEMU_ARCH))..."
	@qemu-system-$(QEMU_ARCH) -machine $(QEMU_MACHINE) -cpu $(QEMU_CPU) -smp 1 -m 128M \
		$(QEMU_BIOS) -kernel $(TARGET) -nographic

# Debug with GDB
debug: $(TARGET)
	@echo "Starting QEMU for debugging..."
	@qemu-system-$(QEMU_ARCH) -machine $(QEMU_MACHINE) -cpu $(QEMU_CPU) -smp 1 -m 128M \
		$(QEMU_BIOS) -kernel $(TARGET) -nographic -S -s

clean:
	@rm -rf build/

# Print configuration
info:
	@echo "ARCH:        $(ARCH)"
	@echo "SUBARCH:     $(SUBARCH)"
	@echo "PLATFORM:    $(PLATFORM)"
	@echo "CROSS_COMPILE: $(CROSS_COMPILE)"
	@echo "BUILD_DIR:   $(BUILD_DIR)"
	@echo "C_SOURCES:   $(C_SOURCES)"
	@echo "ASM_SOURCES: $(ASM_SOURCES)"
