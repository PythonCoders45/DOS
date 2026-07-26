#include "types.h"

#define ACPI_SLP_TYPa 0x2000

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ __volatile__ ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// Root System Description Pointer (RSDP) Header
typedef struct __attribute__((packed)) {
    char signature[8];      // Must match "RSD PTR "
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} ACPI_RSDP;

/**
 * @brief Locates the ACPI RSDP table in lower BIOS RAM memory (0xE0000 - 0xFFFFF)
 */
ACPI_RSDP* find_rsdp(void) {
    uint8_t* mem = (uint8_t*)0xE0000;
    for (uint32_t i = 0; i < 0x20000; i += 16) {
        if (mem[i]   == 'R' && mem[i+1] == 'S' && mem[i+2] == 'D' && mem[i+3] == ' ' &&
            mem[i+4] == 'P' && mem[i+5] == 'T' && mem[i+6] == 'R' && mem[i+7] == ' ') {
            return (ACPI_RSDP*)&mem[i];
        }
    }
    return NULL;
}

/**
 * @brief Executes a software system power-off command
 */
void sys_shutdown(void) {
    // 1. Try QEMU / Bochs ACPI Shutdown port
    outw(0x604, 0x2000);

    // 2. Fallback VirtualBox ACPI Shutdown port
    outw(0x4004, 0x3400);

    // 3. Fallback APM Shutdown
    outw(0xB004, 0x2000);

    // Halt CPU if shutdown succeeds
    while (1) {
        __asm__ __volatile__("hlt");
    }
}
