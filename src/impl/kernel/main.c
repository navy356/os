#include "print.h"
#include "io.h"
#include "init.h"
#include "paging.h"
#include "constants.h"
#include "kheap.h"
#include "modules.h"
#include "terminal.h"

struct Char
{
    uint8_t character;
    uint8_t color;
};

void kernel_main()
{
    struct multiboot_tag *tag;
    void *module;
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        asm("hlt");
    }
    init();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    move_cursor(0, 0);
    print_clear();
    unsigned long addr = (unsigned long)multiboot_ptr + KERNEL_OFFSET;
    if (addr & 7)
    {
        asm("hlt");
    }

    module = findModule();

    /*int a = malloc(0x8, 0);
    int b = malloc(0x8, 1);
    write(hexToString(a));
    write("\n");
    write(hexToString(b));
    write("\n");
    free(a);
    int c = malloc(0x8, 0);
    write(hexToString(c));
    write("\n");*/
    write(hexToString(module));
    callModule(module);

    //startTerminal();
}