#include "init.h"
#include "io.h"
#include "idt.h"
#include "paging.h"
void init()
{
    init_io();
    init_idt();
    init_paging();
}