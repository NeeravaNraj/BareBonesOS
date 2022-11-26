PATH=$(HOME)/opt/cross/bin
TARGET=i686-elf
SRC_DIR=src
BUILD_DIR=build
ASM=$(PATH)/$(TARGET)-as
GCC=$(PATH)/$(TARGET)-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LD=$(PATH)/$(TARGET)-gcc
LINK_FLAGS=-ffreestanding -O2 -nostdlib
GRUB=grub-mkrescue
ISO_DIR=isodir


.PHONY: all always bootloader kernel link create_image

all: bootloader kernel link create_image

bootloader:
	$(ASM) $(SRC_DIR)/boot/boot.asm -o $(BUILD_DIR)/o/boot.o

kernel:
	$(GCC) -c $(SRC_DIR)/kernel/kernel.c -o $(BUILD_DIR)/o/kernel.o $(CFLAGS)

link:
	$(LD) -T $(SRC_DIR)/linker/linker.ld -o $(BUILD_DIR)/bin/myos.bin $(LINK_FLAGS) $(BUILD_DIR)/o/boot.o $(BUILD_DIR)/o/kernel.o -lgcc

always:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(ISO_DIR)/boot/grub

