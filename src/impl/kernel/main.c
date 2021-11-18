#include "print.h"
#include "io.h"
#include "init.h"

struct Char
{
    uint8_t character;
    uint8_t color;
};

void kernel_main() {
    init();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_clear();
    char * buf2 = hexToString(0x1337);
    write(buf2);
}