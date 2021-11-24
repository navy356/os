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
}