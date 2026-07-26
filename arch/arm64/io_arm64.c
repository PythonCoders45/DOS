#include "arch.h"

// Memory-Mapped I/O Base Address for BCM2711 / QEMU 'virt' UART PL011
#define UART0_DR   ((volatile uint32_t*)(0x09000000))
#define UART0_FR   ((volatile uint32_t*)(0x09000018))

void arch_early_init(void) {
    // MMIO registers initialized by hardware bootloader / UEFI
}

void arch_putchar(char c) {
    // Wait until transmit FIFO buffer is not full
    while (*UART0_FR & (1 << 5));
    *UART0_DR = (uint32_t)c; // Write directly to RAM memory-mapped address
}

void arch_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        arch_putchar(str[i]);
    }
}

const char* arch_get_name(void) {
    return "ARM64 (AArch64 Exception Level 1)";
}

void arch_reboot(void) {
    // Send System Control Block PSCI Reboot Call
    asm volatile ("mov x0, #0x84000009\n smc #0");
}

void arch_shutdown(void) {
    // Send System Control Block PSCI Shutdown Call
    asm volatile ("mov x0, #0x84000008\n smc #0");
}
