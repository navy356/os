#pragma once

#include <stdint.h>

struct IDT{
    uint16_t offset_low;
    uint16_t offset_selector;
    uint8_t ist;
    uint8_t types_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
};

typedef struct registers
{
    uint64_t r11, r10, r9, r8, rdx, rcx, rax;
    uint64_t int_no, err_code;
    uint64_t rip, cs, rflags, ss_rsp;
} registers_t;

typedef void (*isr_t)(registers_t);
extern uint64_t isr0;
extern uint64_t isr1;
extern uint64_t isr2;
extern uint64_t isr3;
extern uint64_t isr4;
extern uint64_t isr5;
extern uint64_t isr6;
extern uint64_t isr7;
extern uint64_t isr8;
extern uint64_t isr9;
extern uint64_t isr10;
extern uint64_t isr11;
extern uint64_t isr12;
extern uint64_t isr13;
extern uint64_t isr14;
extern uint64_t isr15;
extern uint64_t isr16;
extern uint64_t isr17;
extern uint64_t isr18;
extern uint64_t isr19;
extern uint64_t isr20;
extern uint64_t isr21;
extern uint64_t isr22;
extern uint64_t isr23;
extern uint64_t isr24;
extern uint64_t isr25;
extern uint64_t isr26;
extern uint64_t isr27;
extern uint64_t isr28;
extern uint64_t isr29;
extern uint64_t isr30;
extern uint64_t isr31;

extern uint64_t irq0;
extern uint64_t irq1;

#define IRQ1 33

isr_t interrupt_handlers[256];
void isr1_handler();
void init_idt();
void setIdtVal(int i,uint64_t offset, uint8_t ist, uint16_t offset_selector,uint8_t types_attr);