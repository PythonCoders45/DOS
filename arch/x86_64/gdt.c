#include "types.h"

// Segment Selectors
#define KERNEL_CODE_SEG 0x08
#define KERNEL_DATA_SEG 0x10
#define USER_DATA_SEG   0x1B // Ring 3 Data (0x18 | RPL 3)
#define USER_CODE_SEG   0x23 // Ring 3 Code (0x20 | RPL 3)

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

static gdt_entry_t gdt[5];

void init_gdt(void) {
    // 0x00: Null Segment
    // 0x08: Kernel Code Segment (Ring 0)
    // 0x10: Kernel Data Segment (Ring 0)
    
    // 0x18: User Data Segment (Ring 3)
    gdt[3].access = 0xF2; // Present, Ring 3, Read/Write
    gdt[3].granularity = 0x00;

    // 0x20: User Code Segment (Ring 3)
    gdt[4].access = 0xFA; // Present, Ring 3, Executable/Read
    gdt[4].granularity = 0x20; // 64-bit Long Mode flag
}
