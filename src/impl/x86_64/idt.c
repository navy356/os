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
    for(int i=0; i<256; i++)
    {
        interrupt_handlers[i]=0;
    }
    setIdtVal(1,((uint64_t)&isr1& 0xffffffffffffffff),0,0x08,0x8e);
    //setIdtVal(14,((uint64_t)&isr14& 0xffffffffffffffff),0,0x08,0x8e);
    register_interrupt_handler(1,isr1_handler);

    PIC_remap(0,8);

    outb(0x21,0xfd);
    outb(0xa1,0xff);
    LoadIDT();
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

    outb(0x20,0x20);
    outb(0xa0,0x20);
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
        write('\n');
    }
}

void irq_handler(registers_t regs)
{
    write("irq handler");
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


void setIdtVal(int i,uint64_t offset, uint8_t ist, uint16_t offset_selector,uint8_t types_attr)
{
    _idt[i].zero = 0;
	_idt[i].offset_low = (uint16_t)((offset));
	_idt[i].offset_mid = (uint16_t)((offset) >> 16);
	_idt[i].offset_high = (uint32_t)((offset) >> 32);
	_idt[i].ist = ist;
	_idt[i].offset_selector = offset_selector;
	_idt[i].types_attr = types_attr;
}