#include "init.h"
#include "io.h"
#include "idt.h"
#include "paging.h"
#include "kheap.h"
void init()
{
    init_io();
    init_idt();
    init_paging();
    init_kheap();
}