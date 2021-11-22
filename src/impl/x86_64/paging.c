#include "paging.h"
#include "kheap.h"
#include "io.h"
#include "print.h"
#include "constants.h"
#include "utility.h"
void init_paging()
{
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

int getPage(uint32_t virtual_addr, uint32_t physical_addr, int make, struct page_t *page_table)
{
    uint8_t offset0 = (uint8_t)((virtual_addr & OFFSET_0_MASK_PHYSICAL) >> 24);
    uint8_t offset1 = (uint8_t)((virtual_addr & OFFSET_1_MASK_PHYSICAL) >> 16);
    uint8_t offset2 = (uint8_t)((virtual_addr & OFFSET_2_MASK_PHYSICAL) >> 8);
    if (offset0 == 0 || offset1 == 0 || offset2 == 0)
    {
        return -1;
    }

    struct page_t *address3 = (struct page_t *)(uintptr_t)getEntryAddress(page_table, 0);
    struct page_t *address2 = (struct page_t *)(uintptr_t)getEntryAddress(address3, 0);
    struct page_t *address1 = (struct page_t *)(uintptr_t)getEntryAddress(address2, 0);

    address2->pages[offset2].entry = (uint32_t)(((uintptr_t)address1 & 0xffffffff) >> 12);
    address3->pages[offset1].entry = (uint32_t)(((uintptr_t)address2 & 0xffffffff) >> 12);
    page_table->pages[offset0].entry = (uint32_t)(((uintptr_t)address3 & 0xffffffff) >> 12);
    write(hexToString(offset0));

    return 1;
}

uint64_t getEntryAddress(struct page_t *page_table, uint8_t offset)
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

void page_fault()
{
   // A page fault has occurred.
   
   // Output an error message.
   write("Page fault! :( ");
}