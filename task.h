#include "types.h"

// Saved CPU state for thread context switching
typedef struct {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rip;    // Instruction Pointer
    uint64_t rflags; // CPU Flags
    uint64_t cr3;    // Page Table Pointer (Process Memory Isolation)
} __attribute__((packed)) cpu_context_t;

typedef struct task {
    uint32_t id;
    uint32_t state; // 0 = Ready, 1 = Running, 2 = Blocked
    cpu_context_t context;
    struct task* next;
} task_t;t
