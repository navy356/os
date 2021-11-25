#include "idt.h"
#include "io_asm.h"
#include "io.h"
#include "print.h"
#include "pic.h"
#include "constants.h"

extern struct IDT _idt[256];

extern void LoadIDT();

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void init_idt()
{
    for (int i = 0; i < 256; i++)
    {
        interrupt_handlers[i] = 0;
    }
    setIdtVal(0, ((uint64_t)&isr0 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(1, ((uint64_t)&isr1 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(2, ((uint64_t)&isr2 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(3, ((uint64_t)&isr3 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(4, ((uint64_t)&isr4 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(5, ((uint64_t)&isr5 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(6, ((uint64_t)&isr6 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(7, ((uint64_t)&isr7 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(8, ((uint64_t)&isr8 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(9, ((uint64_t)&isr9 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(10, ((uint64_t)&isr10 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(11, ((uint64_t)&isr11 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(12, ((uint64_t)&isr12 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(13, ((uint64_t)&isr13 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(14, ((uint64_t)&isr14 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(15, ((uint64_t)&isr15 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(16, ((uint64_t)&isr16 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(17, ((uint64_t)&isr17 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(18, ((uint64_t)&isr18 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(19, ((uint64_t)&isr19 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(20, ((uint64_t)&isr20 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(21, ((uint64_t)&isr21 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(22, ((uint64_t)&isr22 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(23, ((uint64_t)&isr23 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(24, ((uint64_t)&isr24 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(25, ((uint64_t)&isr25 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(26, ((uint64_t)&isr26 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(27, ((uint64_t)&isr27 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(28, ((uint64_t)&isr28 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(29, ((uint64_t)&isr29 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(30, ((uint64_t)&isr30 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(31, ((uint64_t)&isr31 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(32, ((uint64_t)&irq0 & 0xffffffffffffffff), 0, 0x08, 0x8e);
    setIdtVal(33, ((uint64_t)&irq1 & 0xffffffffffffffff), 0, 0x08, 0x8e);

    //setIdtVal(14,((uint64_t)&isr14& 0xffffffffffffffff),0,0x08,0x8e);
    register_interrupt_handler(33, isr1_handler);

    //PIC_remap(0,8);

    //outb(0x21,0xfd);
    //outb(0xa1,0xff);
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    LoadIDT();
    asm("hlt");
}

void isr1_handler()
{
    uint8_t scanCode = inb(0x60);
    uint8_t chr = 0;

    /*if(scanCode) //keydown
    {
        chr = _kkybrd_scancode_std[scanCode];
        //write(hexToString(scanCode));
        //print_char(chr);
    }*/

    keyboard_driver(scanCode);
}

void isr_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else
    {
        write(hexToString(regs.int_no));
        write("\n");
    }
}

void irq_handler(registers_t regs)
{
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void setIdtVal(int i, uint64_t offset, uint8_t ist, uint16_t offset_selector, uint8_t types_attr)
{
    _idt[i].zero = 0;
    _idt[i].offset_low = (uint16_t)((offset));
    _idt[i].offset_mid = (uint16_t)((offset) >> 16);
    _idt[i].offset_high = (uint32_t)((offset) >> 32);
    _idt[i].ist = ist;
    _idt[i].offset_selector = offset_selector;
    _idt[i].types_attr = types_attr;
}