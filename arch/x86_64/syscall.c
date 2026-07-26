#include "types.h"

#define IA32_LSTAR 0xC0000082 // Model-Specific Register for Syscall Handler Address

extern void syscall_entry(void);

static inline void wrmsr(uint32_t msr, uint64_t val) {
    uint32_t low = val & 0xFFFFFFFF;
    uint32_t high = val >> 32;
    asm volatile ("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

/**
 * @brief Registers the kernel's System Call entry point with the CPU
 */
void init_syscalls(void) {
    // Write kernel assembly syscall handler to IA32_LSTAR MSR
    wrmsr(IA32_LSTAR, (uint64_t)syscall_entry);
}
