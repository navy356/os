#include "print.h"
#include "io.h"
#include "init.h"
#include "paging.h"
#include "constants.h"
#include "kheap.h"

struct Char
{
    uint8_t character;
    uint8_t color;
};

void kernel_main()
{
    init();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_clear();
    asm("hlt");
    uint64_t a = kmalloc(8);
    asm("hlt");
    uint64_t b = kmalloc(8);
    uint64_t c = kmalloc(8);
    write("a: ");
    write(hexToString(a));
    write(", b: ");
    write(hexToString(b));
    write("\nc: ");
    write(hexToString(c));

    /*kfree(c);
    kfree(b);
    uint64_t d = kmalloc(12);
    monitor_write(", d: ");
    monitor_write_hex(d);*/
}