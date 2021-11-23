#pragma once
#include <stdint.h>
#include "constants.h"

uint64_t placement_address;

uint64_t kmalloc(uint64_t sz);
uint64_t kmalloc_a(uint64_t sz, int align);
uint64_t kmalloc_ap(uint64_t sz, int align, uint64_t *phys);
void init_kheap();