#pragma once
#include <stdint.h>

#define PRESENT_MASK 1
#define READWRITE_MASK 1<<1
#define MODE_MASK 1<<2
#define PWT_MASK 1<<3
#define PCD_MASK 1<<4
#define ACCESSED_MASK 1<<5
#define DIRTY_MASK 1<<6
#define HUGE_MASK 1<<7
#define GLOBAL_MASK 1<<8
#define ADDR_MASK 0xfffffffffffff000

#define OFFSET_0_MASK_PHYSICAL 0xff8000000000 //0x1ff<<39
#define OFFSET_1_MASK_PHYSICAL 0x7fc0000000 //0x1ff<<30
#define OFFSET_2_MASK_PHYSICAL 0x3fe00000 //0x1ff<<21
#define OFFSET_3_MASK_PHYSICAL 0x1ff000 //0x1ff<<12
#define OFFSET_4_MASK_PHYSICAL 0x1ff
#define SIGN_MASK_1 0xffff000000000000
#define SIGN_MASK_0 0x0000000000000000
struct page_entry_t {
    uint64_t entry;
};

struct page_t {
    struct page_entry_t pages[512];
};

struct PageMap {
    uint16_t page_4_offset : 9;
    uint16_t page_3_offset : 9;
    uint16_t page_2_offset : 9;
    uint16_t page_1_offset : 9;
}memMap;
extern struct page_t * pages;

struct PageMap * memMap_ptr;
uint64_t getPhysical(uint64_t address);
void init_paging();
uint64_t getEntryAddress(struct page_t * page_table, uint16_t offset);
void page_fault();
uint64_t mapPage(uint64_t physical_addr, uint64_t size);
struct page_t * getPageTable();
struct page_t *pages1;
struct page_entry_t *getPageEntry(uint64_t entry);
extern void flush_cr3(struct page_t * addr);
uint64_t virtual_addr_start;
uint64_t setPhysicalFrame(uint64_t *page_table,uint16_t offset);