#include "init.h"
#include "io.h"
#include "idt.h"
#include "paging.h"
#include "kheap.h"
#include "initrd.h"
#include "file.h"
#include "multiboot.h"
void init()
{
    init_io();
    init_idt();
    init_paging();
    init_kheap();
    if (header_start->mods_count > 0)
    {
        uint32_t initrd_location = *((uint32_t *)header_start->mods_addr);
        uint32_t initrd_end = *(uint32_t *)(header_start->mods_addr + 4);
        fs_node_t * fs_root = initialise_initrd(initrd_location);
        init_file(fs_root);
    }
    else
    {
        write("Fail");
        asm("hlt");
    }
}