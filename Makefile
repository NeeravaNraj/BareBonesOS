CROSS=$(HOME)/opt/cross/bin
TARGET=i686-elf

SRC_DIR=src
BUILD_DIR=build
ISO_DIR=isodir

LIB=$(SRC_DIR)/kernel $(SRC_DIR)/kernel/lib
INC=$(SRC_DIR)/kernel/include
CFILES=$(foreach D, $(LIB), $(wildcard $(D)/*.c))
DEPFILES=$(patsubst %.c, %.d, $(CFILES))

ASM:=nasm
GCC:=$(CROSS)/$(TARGET)-gcc
LD:=$(CROSS)/$(TARGET)-gcc
OLINK:=$(CROSS)/$(TARGET)-ld
GRUB=grub-mkrescue

OPT=-O2
LINK_FLAGS=-ffreestanding $(OPT) -nostdlib
DEPFLAGS=-MP -MD
CFLAGS=-c -std=gnu99 -ffreestanding -Wall -Wextra -fno-builtin $(foreach D, $(INC), -I $(D) $(OPT) $(DEPFLAGS))

OS=myos


OBJECTS=$(addprefix $(BUILD_DIR)/o/kernel/, $(notdir $(patsubst %.c, %.o, $(CFILES))))


.PHONY: all iso_image link bootloader kernel always clean

all: iso_image

iso_image: $(BUILD_DIR)/$(OS).bin
$(BUILD_DIR)/$(OS).bin: link
	@echo  Creating ISO file
	@rm $(OS).iso
	@cp $(BUILD_DIR)/bin/$(OS).bin $(ISO_DIR)/boot/$(OS).bin
	@cp $(SRC_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB) -o $(OS).iso $(ISO_DIR)
	@echo  Done successfully.
	@./run.sh

link: bootloader kernel $(BUILD_DIR)
$(BUILD_DIR): $(BUILD_DIR)/bin $(ISO_DIR)/boot/cfg
	@echo Linking
	$(LD) -T $(SRC_DIR)/linker/linker.ld -o $(BUILD_DIR)/bin/$(OS).bin $(LINK_FLAGS) $(BUILD_DIR)/o/boot/boot.o $(BUILD_DIR)/o/kernel/*.o $(BUILD_DIR)/o/kernel/lib/*.o -lgcc

$(BUILD_DIR)/bin:
	@mkdir -p $(BUILD_DIR)/bin

$(ISO_DIR)/boot/cfg:
	@mkdir -p $(ISO_DIR)/boot/cfg
	

bootloader: $(BUILD_DIR)/o/boot/boot.o 
$(BUILD_DIR)/o/boot/boot.o: $(SRC_DIR)/boot/boot.asm
	@mkdir -p $(BUILD_DIR)/o/boot
	@echo Assembling bootloader
	$(ASM) -felf32 $(SRC_DIR)/boot/boot.asm -o $(BUILD_DIR)/o/boot/boot.o

kernel: $(OBJECTS) $(BUILD_DIR)/o/kernel/lib
$(BUILD_DIR)/o/kernel/lib:
	mkdir -p $(BUILD_DIR)/o/kernel/lib
$(OBJECTS): $(CFILES) $(INC)/system.h
	@echo Compiling C files
	@make --no-print-directory -C ./src/kernel
	
clean:
	@rm -rf $(ISO_DIR)
	@rm -rf $(BUILD_DIR)
	@rm $(OS).iso
