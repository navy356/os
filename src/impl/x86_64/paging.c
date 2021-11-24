#include "paging.h"
#include "kheap.h"
#include "io.h"
#include "print.h"
#include "constants.h"
#include "utility.h"
#include "kheap.h"
void init_paging()
{
    virtual_addr_start = 0x0000008000000000;
}

struct page_t *getPageTable()
{
    uint64_t physicalAddr = 0;
    uint64_t virtualAddr = kmalloc_ap(0x1000, 1, &physicalAddr);
    return (struct page_t *)physicalAddr;
}
uint64_t getPhysical(uint64_t address)
{
    struct page_t *page_table = pages;
    uint64_t address_page_table_physical;
    uint64_t address_page_table_1;
    uint64_t address_page_table_2;
    uint64_t address_page_table_3;

    uint16_t offset_level_4 = (uint16_t)((address & OFFSET_0_MASK_PHYSICAL) >> 39);
    uint16_t offset_level_3 = (uint16_t)((address & OFFSET_1_MASK_PHYSICAL) >> 30);
    uint16_t offset_level_2 = (uint16_t)((address & OFFSET_2_MASK_PHYSICAL) >> 21);
    uint16_t offset_level_1 = (uint16_t)((address & OFFSET_3_MASK_PHYSICAL) >> 12);
    uint16_t offset_physical = (uint16_t)((address & OFFSET_4_MASK_PHYSICAL) >> 0);

    address_page_table_3 = getEntryAddress(page_table, offset_level_4);

    if (address_page_table_3 == -1)
    {
        return -1;
    }
    else
    {
        address_page_table_3 = getVirtualKernelOffset(address_page_table_3);
    }
    address_page_table_2 = getEntryAddress((struct page_t *)address_page_table_3, offset_level_3);

    if (address_page_table_2 == -1)
    {
        return -1;
    }
    else
    {
        address_page_table_2 = getVirtualKernelOffset(address_page_table_2);
    }
    address_page_table_1 = getEntryAddress((struct page_t *)address_page_table_2, offset_level_2);
    if (address_page_table_1 == -1)
    {
        return -1;
    }
    else
    {
        address_page_table_1 = getVirtualKernelOffset(address_page_table_1);
    }
    if (address & HUGE_MASK)
    {
        address_page_table_physical = address_page_table_1 + offset_level_1 << 12;
    }
    else
    {
        address_page_table_physical = getEntryAddress((struct page_t *)address_page_table_1, offset_level_1);
        if (address_page_table_physical == -1)
        {
            return -1;
        }
        else
        {
            address_page_table_physical = getVirtualKernelOffset(address_page_table_physical);
        }
    }
    uint64_t physical = address_page_table_physical + offset_physical;

    return physical;
}

uint64_t mapPage(uint64_t physical_addr, uint64_t size)
{
    struct page_t *page_table = pages;

    int pages_needed = 0;
    if (size < 0x1000)
    {
        pages_needed = 1;
        size = 0;
    }
    while (size > 0)
    {
        pages_needed += 1;
        size -= 0x1000;
    }

    uint64_t virtual_addr = virtual_addr_start;
    virtual_addr = virtual_addr+(physical_addr & OFFSET_4_MASK_PHYSICAL);

    while (pages_needed > 0)
    {

        uint16_t offset_level_4 = (virtual_addr_start & OFFSET_0_MASK_PHYSICAL) >> 39;
        uint16_t offset_level_3 = (virtual_addr_start & OFFSET_1_MASK_PHYSICAL) >> 30;
        uint16_t offset_level_2 = (virtual_addr_start & OFFSET_2_MASK_PHYSICAL) >> 21;
        uint16_t offset_level_1 = (virtual_addr_start & OFFSET_3_MASK_PHYSICAL) >> 12;
        uint16_t offset_physical = (physical_addr & OFFSET_4_MASK_PHYSICAL);
        uint16_t recursive_offset = 0x1ff;

        uint64_t sign;
        sign = virtual_addr | SIGN_MASK_1;
        sign = sign & SIGN_MASK_1;

        //level 1 table entry
        uint64_t *lv1 = (uint64_t *)(sign + (((uint64_t)recursive_offset << 39)) + (((uint64_t)offset_level_4 << 30)) + ((uint64_t)(offset_level_3 << 21)) + (((uint64_t)offset_level_2 << 12)) + (((uint64_t)offset_level_1 * 8)));
        uint64_t *lv2 = (uint64_t *)(sign + ((uint64_t)recursive_offset << 39) + ((uint64_t)recursive_offset << 30) + ((uint64_t)offset_level_4 << 21) + ((uint64_t)offset_level_3 << 12) + ((uint64_t)offset_level_2 * 8));
        uint64_t *lv3 = (uint64_t *)(sign + ((uint64_t)recursive_offset << 39) + ((uint64_t)recursive_offset << 30) + ((uint64_t)recursive_offset << 21) + ((uint64_t)offset_level_4 << 12) + ((uint64_t)offset_level_3 * 8));
        uint64_t *lv4 = (uint64_t *)(sign + (((uint64_t)recursive_offset << 39)) + (((uint64_t)recursive_offset << 30)) + (((uint64_t)recursive_offset << 21)) + (((uint64_t)recursive_offset << 12)) + ((uint64_t)offset_level_4 * 8));

        setPhysicalFrame(lv4, 0);
        setPhysicalFrame(lv3, 0);
        setPhysicalFrame(lv2, 0);
        *lv1 = (physical_addr & 0xFFFFFFFFFFFFF000) | 0b11;
        physical_addr += 0x1000;
        virtual_addr_start += 0x1000;
        pages_needed -= 1;
    }

    flush_cr3((struct page_t *)getPhysicalKernelOffset((uint64_t)page_table));

    return virtual_addr;
}

uint64_t getEntryAddress(struct page_t *page_table, uint16_t offset)
{
    struct page_entry_t page_entry = page_table->pages[offset];
    if (page_entry.entry & PRESENT_MASK)
    {
        uint64_t entry = (page_entry.entry & ADDR_MASK);
        return entry;
    }
    else
    {
        return -1;
    }
}

uint64_t setPhysicalFrame(uint64_t *page_table, uint16_t offset)
{
    uint64_t physical = *((uint64_t *)page_table);

    if ((physical & PRESENT_MASK) == 0)
    {
        kmalloc_ap(0x1000, 1, &physical);
        *page_table = physical | 0b11;
    }
    return physical;
}

void page_fault()
{
    // A page fault has occurred.

    // Output an error message.
    write("Page fault! :( ");
}