CROSS=$(HOME)/opt/cross/bin
TARGET=i686-elf

DIR = $(shell cd ../.. && pwd && cd src/kernel)
GCC:=$(CROSS)/$(TARGET)-gcc
BUILD_DIR=build

CFLAGS=-c -std=gnu99 -ffreestanding -Wall -Wextra -fno-builtin -I include

SRC_FILES = $(shell find . -type f -name "*.c")
OBJ_FILES = $(SRC_FILES:.c=.o)

all: $(OBJ_FILES)

%.o: %.c
	@$(GCC) $(CFLAGS) $(DIR)/src/kernel/$< -o $(DIR)/$(BUILD_DIR)/o/kernel/$@

