# Usage:
#   make ARCH=x86_64
#   make ARCH=arm64

ARCH ?= x86_64

ifeq ($(ARCH), x86_64)
    CC = x86_64-elf-gcc
    AS = nasm
    ASFLAGS = -f elf64
    ARCH_OBJS = arch/x86_64/boot64.o arch/x86_64/io64.o
    TARGET = kernel_x64.bin
else ifeq ($(ARCH), arm64)
    CC = aarch64-none-elf-gcc
    AS = aarch64-none-elf-as
    ASFLAGS =
    ARCH_OBJS = arch/arm64/boot_arm64.o arch/arm64/io_arm64.o
    TARGET = kernel_arm64.bin
endif

COMMON_OBJS = main.o shell.o dos_commands.o separater.o percentage.o

all: $(TARGET)

$(TARGET): $(ARCH_OBJS) $(COMMON_OBJS)
	$(CC) -T linker.ld -o $(TARGET) -ffreestanding -O2 -nostdlib $(ARCH_OBJS) $(COMMON_OBJS)
