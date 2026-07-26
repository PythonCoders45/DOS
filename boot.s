; Declare Multiboot Header constants
MBALIGN     equ  1 << 0                   ; Align loaded modules on page boundaries
MEMINFO     equ  1 << 1                   ; Provide memory map
FLAGS       equ  MBALIGN | MEMINFO        ; Multiboot 'flag' field
MAGIC       equ  0x1BADB002               ; Magic number letting bootloader find header
CHECKSUM    equ -(MAGIC + FLAGS)          ; Checksum required by Multiboot standard

; Multiboot Header
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; Allocate 16 KB stack for C code
section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB
stack_top:

; Kernel Entry Point
section .text
global _start
extern kernel_main

_start:
    ; Set up the stack pointer register (ESP)
    mov esp, stack_top

    ; Call our main kernel function in C!
    call kernel_main

    ; If kernel_main unexpectedly returns, halt CPU in an infinite loop
.halt:
    cli
    hlt
    jmp .halt
