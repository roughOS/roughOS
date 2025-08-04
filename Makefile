CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Ikernel -g -mgeneral-regs-only
LD = ld
LDFLAGS = -m elf_i386 -nostdlib -T linker.ld

NASM = nasm
NASMFLAGS = -f elf32

KERNEL_DIR = kernel
BUILD_DIR = build

C_SOURCES := $(shell find $(KERNEL_DIR) -name '*.c')
ASM_SOURCES := $(shell find $(KERNEL_DIR) -name '*.asm')

C_OBJECTS := $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS := $(patsubst $(KERNEL_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

all: $(BUILD_DIR)/roughos.elf

$(BUILD_DIR)/roughos.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean