global isr1
global LoadIDT
extern _idt
extern inb
extern isr1_handler
extern gdt64

section.text
    bits 64

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

idtDescriptor:
    dw 4095
    dq _idt

isr1:
    PUSHALL
    call isr1_handler
    POPALL
    iretq

LoadIDT:
    PUSHALL
    lidt[idtDescriptor]
    sti
    POPALL
    ret