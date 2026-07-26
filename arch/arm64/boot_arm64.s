.section .text.boot
.global _start_arm64

_start_arm64:
    ; Read CPU ID, halt secondary cores (run only on Core 0)
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbnz    x0, .halt

    ; Set up 64-bit Stack Pointer (SP)
    ldr     x0, =_stack_top_arm64
    mov     sp, x0

    ; Jump to architecture-independent main.c entry point
    bl      main

.halt:
    wfe
    b       .halt

.section .bss
.align 16
_stack_bottom_arm64:
    .skip 65536 ; 64 KB Stack
_stack_top_arm64:
