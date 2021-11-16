#include "init.h"
#include "io.h"
#include "idt.h"
void init()
{
    init_io();
    //init_idt();
}