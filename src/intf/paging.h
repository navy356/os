#pragma once
#include <stdint.h>

#define PRESENT_MASK 1
#define READWRITE_MASK 1<<1
#define MODE_MASK 1<<2
#define PWT_MASK 1<<3
#define PCD_MASK 1<<4
#define ACCESSED_MASK 1<<5
#define DIRTY_MASK 1<<6
#define PAT_MASK 1<<7
#define GLOBAL_MASK 1<<8
#define ADDR_MASK 0xfffff000

#define OFFSET_0_MASK 0xff000000
#define OFFSET_1_MASK 0x00ff0000
#define OFFSET_2_MASK 0x0000ff00
#define OFFSET_3_MASK 0x000000ff

struct page_entry_t {
    uint32_t entry;
};

struct page_t {
    struct page_entry_t pages[512];
};

extern struct page_t * pages;

uint32_t getPhysical(uint32_t address, struct page_t * page_table);
void init_paging();
uint32_t getEntryAddress(struct page_t * page_table, uint8_t offset);

struct page_t *pages1;