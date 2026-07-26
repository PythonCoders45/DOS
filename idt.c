#include "types.h"

// IDT Entry Structure
struct IDTEntry {
    uint16_t base_low;
    uint16_t selector;  // Kernel code segment offset (0x08)
    uint8_t  zero;
    uint8_t  flags;     // Privilege level flags (0xEE for Ring 3 access)
    uint16_t base_high;
} __attribute__((packed));

struct IDTPtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct IDTEntry idt[256];
struct IDTPtr idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = sel;
    idt[num].zero      = 0;
    idt[num].flags     = flags;
}

// System Call Dispatcher Handler (Triggered via software interrupt 'int 0x80')
void syscall_handler(uint32_t syscall_num, uint32_t arg1, uint32_t arg2) {
    switch (syscall_num) {
        case 1: // Syscall 1: Draw Pixel
            // draw_pixel(arg1, arg2, color...);
            break;
        case 2: // Syscall 2: Play Sound
            // play_sound(arg1);
            break;
        case 3: // Syscall 3: Exit Program
            // terminate_process();
            break;
        default:
            break;
    }
}

void init_idt(void) {
    idtp.limit = (sizeof(struct IDTEntry) * 256) - 1;
    idtp.base  = (uint32_t)&idt;

    // Register syscall gate at Interrupt 0x80 with Ring 3 privilege flags (0xEE)
    // idt_set_gate(0x80, (uint32_t)syscall_stub, 0x08, 0xEE);

    // Load IDT into CPU using assembly 'lidt'
    __asm__ __volatile__("lidt %0" : : "m"(idtp));
}
