#include "print.h"
#include "io.h"
#include "init.h"
#include "paging.h"
#include "constants.h"
#include "kheap.h"
#include "file.h"

struct Char
{
    uint8_t character;
    uint8_t color;
};

void kernel_main()
{
    init();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    move_cursor(0, 0);
    print_clear();
    int i = 0;
    struct dirent *node = 0;
    while ((node = readdir_fs(fs_root, i)) != 0)
    {
        write("Found file ");
        write(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags & 0x7) == FS_DIRECTORY)
            write("\n\t(directory)\n");
        else
        {
            write("\n\t contents: \"");
            char buf[256];
            uint64_t sz = read_fs(fsnode, 0, 256, buf);
            int j;
            //for (j = 0; j < sz; j++)
            //    write(buf[j]);

            write("\"\n");
        }
        i++;
    }
}