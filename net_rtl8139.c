#include "types.h"

#define RTL8139_IO_PORT 0xC000 // Base I/O port assigned via PCI bus scan

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ __volatile__ ("outl %0, %1" : : "a"(val), "Nd"(port));
}

// 8 KB RX Ring Buffer for incoming Ethernet packets
uint8_t rx_buffer[8192 + 16 + 1536];
uint8_t mac_address[6];

void init_rtl8139(void) {
    // 1. Turn on the card (Power ON register 0x52 = 0x00)
    outb(RTL8139_IO_PORT + 0x52, 0x00);

    // 2. Software Reset (Register 0x37 = 0x10)
    outb(RTL8139_IO_PORT + 0x37, 0x10);
    while ((inb(RTL8139_IO_PORT + 0x37) & 0x10) != 0); // Wait for reset to complete

    // 3. Set up RX Ring Buffer memory address (Register 0x30)
    outl(RTL8139_IO_PORT + 0x30, (uint32_t)&rx_buffer);

    // 4. Enable Interrupts (TOK and ROK bits in register 0x3C)
    outb(RTL8139_IO_PORT + 0x3C, 0x0005);

    // 5. Configure Receiver (Accept Broadcast, Multicast, and Physical Match packets)
    outl(RTL8139_IO_PORT + 0x44, 0x0F);

    // 6. Enable Receiver & Transmitter (Register 0x37 = 0x0C)
    outb(RTL8139_IO_PORT + 0x37, 0x0C);

    // 7. Read Hardware MAC Address from Registers 0x00 - 0x05
    for (int i = 0; i < 6; i++) {
        mac_address[i] = inb(RTL8139_IO_PORT + i);
    }
}
