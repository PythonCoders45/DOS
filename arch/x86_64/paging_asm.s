bits 64
global load_pml4

section .text
load_pml4:
    ; Function argument passed in RDI register (System V ABI)
    mov cr3, rdi    ; Load Page Map Level 4 base pointer into CR3
    ret
