#include "print.h"
#include "io.h"
#include "init.h"
#include "paging.h"
#include "constants.h"

struct Char
{
    uint8_t character;
    uint8_t color;
};

void kernel_main() {
    init();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_clear();
    mapPage(0x00000,0x200000*2);
}