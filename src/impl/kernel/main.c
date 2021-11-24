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
    move_cursor(0,0);
    print_clear();
    write("I work");
    write("\n");
    uint64_t a = malloc(0x100,0);
    write(hexToString(a));
    write("\n");
    uint64_t b = malloc(0x10,1);
    write(hexToString(b));
    write("\n");
    free(a);
    free(b);
    write("hmmm");
    uint64_t c = kmalloc_a(0x10,0);
    write(hexToString(c));
    write("\n");
    asm("hlt");

    /*kfree(c);
    kfree(b);
    uint64_t d = kmalloc(12);
    monitor_write(", d: ");
    monitor_write_hex(d);*/
}