#include "paging.h"
#include "kheap.h"
#include "io.h"
#include "print.h"
#include "constants.h"
#include "utility.h"
#include "kheap.h"
void init_paging()
{
    memMap_ptr = &memMap;
    memMap_ptr->page_4_offset = 1;
    memMap_ptr->page_3_offset = 0;
    memMap_ptr->page_2_offset = 0;
    memMap_ptr->page_1_offset = 0;
}

void fixMap(int count)
{
    while (count > 0)
    {
        memMap_ptr->page_1_offset+=1;
        count-=1;
        if (memMap_ptr->page_1_offset >= 512)
        {
            memMap_ptr->page_1_offset = 0;
            memMap_ptr->page_2_offset += 1;
            if (memMap_ptr->page_2_offset >= 512)
            {
                memMap_ptr->page_2_offset = 0;
                memMap_ptr->page_3_offset += 1;
                if (memMap_ptr->page_3_offset >= 512)
                {
                    memMap_ptr->page_3_offset = 0;
                    memMap_ptr->page_4_offset += 1;
                    if (memMap_ptr->page_4_offset >= 512)
                    {
                        write("Memory full\n");
                        asm("hlt");
                    }
                }
            }
        }
    }
}

struct page_t * getPageTable()
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

    uint64_t o4 = (uint64_t)memMap_ptr->page_4_offset;
    uint64_t o3 = (uint64_t)memMap_ptr->page_3_offset;
    uint64_t o2 = (uint64_t)memMap_ptr->page_2_offset;
    uint64_t virtual_addr = (o4<<39)+(o3<<30)+(o2<<21);

    int pages_needed = 0;
    while (size > 0)
    {
        pages_needed += 1;
        size -= 0x1000;
    }

    if (pages_needed >= 512)
    {
        virtual_addr = virtual_addr+(physical_addr&OFFSET_3_MASK_PHYSICAL)+(physical_addr&OFFSET_4_MASK_PHYSICAL);
    }
    else
    {
        virtual_addr = virtual_addr+memMap_ptr->page_1_offset+(physical_addr&OFFSET_4_MASK_PHYSICAL);
    }

    write(hexToString(pages_needed));
    write("\n");
    write(hexToString(virtual_addr));
    write("\n");

    while (pages_needed > 0)
    {
        if (pages_needed >= 512)
        { //assumption we never free these
            uint64_t entry_pt3 = getEntryAddress(page_table, memMap_ptr->page_4_offset);
            if (entry_pt3 == -1)
            {
                entry_pt3 = (uint64_t)getPageTable();
                setEntryAddress((struct page_t *)page_table, memMap_ptr->page_4_offset, entry_pt3|0b11);
            }
            uint64_t entry_pt2 = getEntryAddress((struct page_t *)getVirtualKernelOffset(entry_pt3), memMap_ptr->page_3_offset);
            if (entry_pt2 == -1)
            {
                entry_pt2 = (uint64_t)getPageTable();
                setEntryAddress((struct page_t *)getVirtualKernelOffset(entry_pt3), memMap_ptr->page_3_offset, entry_pt2|0b11);
            }
            //fix mask
            uint64_t entry = 0xFFE00000 & physical_addr;
            entry = entry | 0b10000011;
            uint64_t entry_pt1 = (uint64_t)entry;
            setEntryAddress((struct page_t *)getVirtualKernelOffset(entry_pt2), memMap_ptr->page_1_offset, entry_pt1);
            pages_needed -= 512;
            physical_addr = physical_addr + 0x200000;
            fixMap(512);
            write(hexToString(memMap_ptr->page_1_offset));
            write("\n");
            write(hexToString(memMap_ptr->page_2_offset));
            write("\n");
            write(hexToString(memMap_ptr->page_3_offset));
            write("\n");
            write(hexToString(memMap_ptr->page_4_offset));
            write("\n");
        }
        else
        {
            uint64_t entry_pt3 = getEntryAddress((struct page_t *)page_table, memMap_ptr->page_4_offset);
            if (entry_pt3 == -1)
            {
                entry_pt3 = (uint64_t)getPageTable();
                setEntryAddress((struct page_t *)page_table, memMap_ptr->page_4_offset, entry_pt3|0b11);
            }
            uint64_t entry_pt2 = getEntryAddress((struct page_t *)getVirtualKernelOffset(entry_pt3), memMap_ptr->page_3_offset);
            if (entry_pt2 == -1)
            {
                entry_pt2 = (uint64_t)getPageTable();
                setEntryAddress((struct page_t *)getVirtualKernelOffset(entry_pt3), memMap_ptr->page_3_offset, entry_pt2|0b11);
            }
            uint64_t entry_pt1 = getEntryAddress((struct page_t *)getVirtualKernelOffset(entry_pt2), memMap_ptr->page_2_offset);
            if (entry_pt1 == -1)
            {
                entry_pt1 = (uint64_t)getPageTable();
                setEntryAddress((struct page_t *)getVirtualKernelOffset(entry_pt2), memMap_ptr->page_2_offset, entry_pt1|0b11);
            }
            uint64_t entry = 0xFFFFF000 & physical_addr;
            entry = entry | 0b11;
            uint64_t entry_pt0 = (uint64_t)entry;
            setEntryAddress((struct page_t *)getVirtualKernelOffset(entry_pt1), memMap_ptr->page_1_offset, entry_pt0);
            pages_needed -= 1;
            physical_addr = physical_addr + 0x1000;
            fixMap(1);
        }
    }

    flush_cr3(getPhysicalKernelOffset(page_table));
    

    return virtual_addr;
}

uint64_t setEntryAddress(struct page_t *page_table, uint16_t offset, uint64_t address)
{
    page_table->pages[offset].entry = address;
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

void page_fault()
{
    // A page fault has occurred.

    // Output an error message.
    write("Page fault! :( ");
}

struct page_entry_t *getPageEntry(uint64_t entry)
{
    uint64_t physicalAddr = 0;
    uint64_t virtualAddr = kmalloc_ap(sizeof(struct page_entry_t), 1, &physicalAddr);
    write(hexToString(virtualAddr));
    write("\n");
    struct page_entry_t *pe = (struct page_entry_t *)virtualAddr;
    
    pe->entry = entry;
    return pe;
}