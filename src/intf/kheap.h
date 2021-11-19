#pragma once
#include <stdint.h>

extern uint32_t placement_address;

uint32_t kmalloc(uint32_t sz);
uint32_t kmalloc_a(uint32_t sz, int align);
uint32_t kmalloc_ap(uint32_t sz, int align, uint32_t *phys);