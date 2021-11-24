global LoadIDT
extern _idt
extern inb
extern isr1_handler
extern isr_handler
extern irq_handler
extern gdt64
extern kernel_offset

section.text
    bits 64

    %macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        cli                         ; Disable interrupts firstly.
        push byte 0                 ; Push a dummy error code.
        push byte %1                ; Push the interrupt number.
        jmp isr_common_stub         ; Go to our common handler code.
    %endmacro
    
    %macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli                         ; Disable interrupts.
        push byte %1                ; Push the interrupt number
        jmp isr_common_stub
    %endmacro

    %macro IRQ 2
    global irq%1
    irq%1:
        cli
        push byte 0
        push byte %2
        jmp irq_common_stub
    %endmacro

    ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

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
        pop rax
        pop rax
    %endmacro

idtDescriptor:
    dw 4095
    dq _idt

LoadIDT:
    mov rax,idtDescriptor
    sub rax,kernel_offset
    lidt[idtDescriptor]
    sti
    ret

isr_common_stub:
    PUSHALL
    call isr_handler
    POPALL
    sti
    iretq      

irq_common_stub:
    PUSHALL
    call irq_handler
    POPALL
    sti
    iretq      
