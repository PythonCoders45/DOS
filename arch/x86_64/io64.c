#include "arch.h"

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void arch_early_init(void) {
    // Initialize COM1 Serial Port (0x3F8)
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03); // 38400 baud rate
    outb(0x3F8 + 3, 0x03);
}

void arch_putchar(char c) {
    outb(0x3F8, c);
}

void arch_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        arch_putchar(str[i]);
    }
}

const char* arch_get_name(void) {
    return "x86-64 (Long Mode 64-Bit)";
}

void arch_reboot(void) {
    // Pulse keyboard controller to trigger CPU reset
    outb(0x64, 0xFE);
}

void arch_shutdown(void) {
    // QEMU ACPI poweroff port
    asm volatile ("outw %0, %1" : : "a"((uint16_t)0x2000), "Nd"((uint16_t)0x0604));
}
