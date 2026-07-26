global syscall_stub
extern syscall_handler

section .text

syscall_stub:
    pushad               ; Save EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI

    ; Pass arguments from registers to C syscall handler
    push edx             ; Argument 2
    push ecx             ; Argument 1
    push eax             ; Syscall Number
    call syscall_handler ; Call C dispatcher function in idt.c
    add esp, 12          ; Clean up pushed arguments from stack

    popad                ; Restore general purpose registers
    iretd                ; Return from interrupt (Restores CS, EIP, EFLAGS)
