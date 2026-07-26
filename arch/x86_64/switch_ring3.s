bits 64
global jump_to_user_mode

section .text
jump_to_user_mode:
    ; Arg 1 (RDI): User function entry point address
    ; Arg 2 (RSI): User stack pointer address

    mov ax, 0x1B        ; User Data Segment (0x18 | RPL 3)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Push fake Interrupt Stack Frame for iretq
    push 0x1B           ; SS: User Data Segment
    push rsi            ; RSP: User Stack Pointer
    pushf               ; RFLAGS
    pop rax
    or rax, 0x200       ; Enable Interrupts (IF flag)
    push rax
    push 0x23           ; CS: User Code Segment (0x20 | RPL 3)
    push rdi            ; RIP: User Entry Point Address

    iretq               ; Execute switch to Ring 3!
