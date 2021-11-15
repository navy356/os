    global outb             ; make the label outb visible outside this file

    ; outb - send a byte to an I/O port
    outb:
        mov rax, rsi
        ;mov al, edi    ; move the data to be sent into the al register
        mov rdx, rdi
        ;mov dx, esi    ; move the address of the I/O port into the dx register
        out dx, al           ; send the data to the I/O port
        ret
        