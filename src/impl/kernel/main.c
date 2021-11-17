#include "print.h"
#include "io.h"
#include "init.h"

void kernel_main() {
    init();
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    char * buf2 = hexToString(0x1337);
    write(buf2);
}