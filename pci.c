#include "types.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ __volatile__ ("outl %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ __volatile__ ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Reads 32 bits from PCI Configuration Space
 */
uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) |
                       (func << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

/**
 * @brief Scans all 256 PCI buses to detect hardware devices (GPUs, NICs, Sound cards)
 */
void scan_pci_bus(void) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t func = 0; func < 8; func++) {
                uint32_t vendor_device = pci_read(bus, slot, func, 0x00);
                uint16_t vendor_id = vendor_device & 0xFFFF;
                uint16_t device_id = (vendor_device >> 16) & 0xFFFF;

                // 0xFFFF indicates no device present at this slot
                if (vendor_id != 0xFFFF) {
                    // Read Class & Subclass code (Offset 0x08)
                    uint32_t class_reg = pci_read(bus, slot, func, 0x08);
                    uint8_t class_code = (class_reg >> 24) & 0xFF;
                    uint8_t subclass   = (class_reg >> 16) & 0xFF;

                    // Read Base Address Register 0 (BAR0)
                    uint32_t bar0 = pci_read(bus, slot, func, 0x10);

                    // Device detected! (Logging can be routed to serial_print)
                    (void)class_code;
                    (void)subclass;
                    (void)bar0;
                    (void)device_id;
                }
            }
        }
    }
}
