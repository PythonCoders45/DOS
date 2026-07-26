#include "types.h"

typedef struct {
    uint8_t ac_status;       // 0 = Battery, 1 = Plugged in (AC Power), 0xFF = Unknown
    uint8_t battery_status;  // 0 = High, 1 = Low, 2 = Critical, 3 = Charging
    uint8_t battery_percent; // 0% - 100% (0xFF = Unknown)
} PowerStatus;

// Basic ACPI/QEMU Power Off trigger via Port 0x604
void power_off(void) {
    // Shutdown command sent to QEMU / Bochs emulator ACPI port
    outb(0x604, 0x00); // Magic shutdown value
    outb(0x604, 0x20); 
    
    // Fallback APM shutdown via port 0xB004
    outb(0xB004, 0x2000);
}
