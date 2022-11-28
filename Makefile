CROSS=$(HOME)/opt/cross/bin
TARGET=i686-elf
SRC_DIR=src
BUILD_DIR=build
ASM=$(CROSS)/$(TARGET)-as
GCC=$(CROSS)/$(TARGET)-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LD=$(CROSS)/$(TARGET)-gcc
LINK_FLAGS=-ffreestanding -O2 -nostdlib
GRUB=grub-mkrescue
ISO_DIR=isodir
OS=myos


.PHONY: all iso_image link bootloader kernel always clean

all: iso_image

iso_image: $(BUILD_DIR)/$(OS).bin
$(BUILD_DIR)/$(OS).bin: link
	cp $(BUILD_DIR)/$(OS).bin $(ISO_DIR)/boot/$(OS).bin
	cp $(SRC_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB) -o $(OS).iso $(ISO_DIR)
	./run.sh

link: bootloader kernel $(BUILD_DIR)
$(BUILD_DIR): always
	$(LD) -T $(SRC_DIR)/linker/linker.ld -o $(BUILD_DIR)/$(OS).bin $(LINK_FLAGS) $(BUILD_DIR)/o/boot.o $(BUILD_DIR)/o/kernel.o -lgcc

bootloader: $(BUILD_DIR)/o
$(BUILD_DIR)/o:
	mkdir -p $(BUILD_DIR)/o
	$(ASM) $(SRC_DIR)/boot/boot.asm -o $(BUILD_DIR)/o/boot.o

kernel: $(BUILD_DIR)/o
	$(GCC) -c $(SRC_DIR)/kernel/kernel.c -o $(BUILD_DIR)/o/kernel.o $(CFLAGS)

always:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(ISO_DIR)/boot/grub

clean:
	rm -rf $(ISO_DIR)
	rm -rf $(BUILD_DIR)
	rm $(OS).iso
