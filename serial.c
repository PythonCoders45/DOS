#include "types.h"

#define COM1 0x3F8

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Initialize COM1 Serial Port (Baud rate 38400)
void init_serial(void) {
    outb(COM1 + 1, 0x00);    // Disable interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);    // Set divisor to 3 (38400 baud)
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
    outb(COM1 + 4, 0x0B);    // Enable IRQs, RTS/DSR set
}

int is_transmit_empty(void) {
    return inb(COM1 + 5) & 0x20;
}

// Write a character to the debug serial log
void write_serial(char a) {
    while (is_transmit_empty() == 0);
    outb(COM1, a);
}

void serial_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        write_serial(str[i]);
    }
}
