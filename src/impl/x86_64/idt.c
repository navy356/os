#include "idt.h"
#include "io_asm.h"
#include "io.h"
#include "print.h"

extern struct IDT _idt[256];
extern uint64_t isr1;
extern void LoadIDT();

void init_idt()
{
    for(uint64_t t=0; t<256; t++)
    {
        _idt[t].zero = 0;
        _idt[t].offset_low = (uint16_t)(((uint64_t)&isr1 & 0x000000000000ffff));
        _idt[t].offset_mid = (uint16_t)(((uint64_t)&isr1 & 0x00000000ffff0000) >> 16);
        _idt[t].offset_high = (uint16_t)(((uint64_t)&isr1 & 0xffffffff00000000) >> 32);
        _idt[t].ist = 0;
        _idt[t].offset_selector = 0x08;
        _idt[t].types_attr = 0x8e;
    }

    outb(0x21,0xfd);
    outb(0xa1,0xff);
    LoadIDT();
}

void isr1_handler()
{
    outb(0x20,0x20);
    outb(0xa0,0x20);
    write('test');
    return;
    //write(hexToString(inb(0x60)));
    //outb(0x20,0x20);
    //outb(0xa0,0x20);
}