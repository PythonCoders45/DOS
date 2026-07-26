bits 64
global _start64
extern main

section .text
_start64:
    ; Reload data segment selectors for Long Mode
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Initialize 64-bit Stack Pointer (RSP)
    mov rsp, stack_top64

    ; Jump to architecture-independent main.c entry point
    call main

.halt:
    hlt
    jmp .halt

section .bss
align 16
stack_bottom64:
    resb 65536 ; 64 KB Stack
stack_top64:
