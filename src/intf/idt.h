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

void isr1_handler();
void init_idt();
void setIdtVal(int i,uint64_t offset, uint8_t ist, uint16_t offset_selector,uint8_t types_attr);