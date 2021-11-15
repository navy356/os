#include "print.h"
#include "io.h"
#include "init.h"

void kernel_main() {
    init();
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    move_cursor(24,79);
    char * buf = "a";
    write(buf,sizeof(buf));
}