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
        pop rax
        pop rcx
        pop rdx
        pop r8
        pop r9
        pop r10
        pop r11
    %endmacro

idtDescriptor:
    dw 4095
    dq _idt

isr1:
    PUSHALL
    push rax
    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call isr1_handler
    pop rax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    POPALL
    iretq

LoadIDT:
    lidt[idtDescriptor]
    sti
    ret