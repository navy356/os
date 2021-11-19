#include "paging.h"
#include "kheap.h"
#include "io.h"
#include "print.h"
#include "constants.h"
#include "utility.h"
void init_paging()
{
}
uint32_t getPhysical(uint32_t address, struct page_t * page_table)
{
    write(hexToString(KERNEL_OFFSET));
    write("\n");
    write(hexToString(page_table));
    write("\n");

    uint8_t offset0 = (uint8_t)((address & OFFSET_0_MASK) >>24);
    uint8_t offset1 = (uint8_t)((address & OFFSET_1_MASK) >>16);
    uint8_t offset2 = (uint8_t)((address & OFFSET_2_MASK) >>8);
    uint8_t offset3 = (uint8_t)(address & OFFSET_3_MASK);

    uint64_t address1 = getVirtualKernelOffset(getEntryAddress(page_table,offset0));
    write(hexToString(address1));
    write("\n");
    asm("hlt");
    uint32_t address2 = getVirtualKernelOffset(getEntryAddress((struct page_t *)(uintptr_t)address1,offset1));
    uint32_t address3 = getVirtualKernelOffset(getEntryAddress((struct page_t *)(uintptr_t)address2,offset2));
    uint32_t physical = address3+offset3;

    physical = (physical&ADDR_MASK)>>12;

    return physical;
}

int getPage(uint32_t virtual_addr,uint32_t physical_addr, int make, struct page_t * page_table)
{
    uint8_t offset0 = (uint8_t)((virtual_addr & OFFSET_0_MASK) >>24);
    uint8_t offset1 = (uint8_t)((virtual_addr & OFFSET_1_MASK) >>16);
    uint8_t offset2 = (uint8_t)((virtual_addr & OFFSET_2_MASK) >>8);
    if(offset0 == 0||offset1==0||offset2==0)
    {
        return -1;
    }

    struct page_t * address3 = (struct page_t *) (uintptr_t)getEntryAddress(page_table,0);
    struct page_t * address2 = (struct page_t *) (uintptr_t)getEntryAddress(address3,0);
    struct page_t * address1 = (struct page_t *) (uintptr_t)getEntryAddress(address2,0);

    address2->pages[offset2].entry = (uint32_t)(((uintptr_t)address1 & 0xffffffff)>>12);
    address3->pages[offset1].entry = (uint32_t)(((uintptr_t)address2 & 0xffffffff)>>12);
    page_table->pages[offset0].entry = (uint32_t)(((uintptr_t)address3 & 0xffffffff)>>12);
    write(hexToString(offset0));

    return 1;
}

uint32_t getEntryAddress(struct page_t * page_table, uint8_t offset)
{
    struct page_entry_t page_entry = page_table->pages[offset];
    write(hexToString(offset));
    write("\n");
    write(hexToString(page_entry.entry));
    write("\n");
    uint64_t entry = (page_entry.entry & ADDR_MASK);
    write(hexToString(entry));
    write("\n");
    return entry;
}