global inb
global outb
global load_idt

section .text

; Read byte from I/O port: inb(port)
inb:
    mov dx, [esp + 4]    ; Get port number from stack argument
    in al, dx            ; Read byte into AL register
    ret                  ; Return value in AL

; Write byte to I/O port: outb(port, value)
outb:
    mov dx, [esp + 4]    ; Get port number
    mov al, [esp + 8]    ; Get value to write
    out dx, al           ; Send byte to hardware port
    ret

; Load Interrupt Descriptor Table pointer: load_idt(idtp_address)
load_idt:
    mov edx, [esp + 4]   ; Get address of IDT pointer
    lidt [edx]           ; Execute x86 LIDT instruction
    sti                  ; Enable hardware interrupts
    ret
