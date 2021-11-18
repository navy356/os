    global outb             ; make the label outb visible outside this file
    global inb

    %macro PUSHALL 0
        push rax
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
    %endmacro

    %macro POPALL 0
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx 
        pop rcx
        pop rax
    %endmacro
    ; outb - send a byte to an I/O port
    outb:
        PUSHALL
        mov rax, rsi
        ;mov al, edi    ; move the data to be sent into the al register
        mov rdx, rdi
        ;mov dx, esi    ; move the address of the I/O port into the dx register
        out dx, al           ; send the data to the I/O port
        POPALL
        ret

    ; inb - returns a byte from the given I/O port
    ; stack: [esp + 4] The address of the I/O port
    ;        [esp    ] The return address
    inb:
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
        mov rdx, rdi       ; move the address of the I/O port to the dx register
        in  al, dx              ; read a byte from the I/O port and store it in the al register
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx 
        pop rcx
        ret                     ; return the read byte