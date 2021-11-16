#include "io_asm.h"
#include "io.h"
#include "constants.h"
#include "print.h"
#include <stdint.h>

/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

struct Cursor cursor;

struct Cursor *cursor_ptr;

void init_io()
{
    cursor_ptr = &cursor;
    cursor_ptr->x = 0;
    cursor_ptr->y = 0;
}

/** move_cursor:
     *  Moves the cursor of the framebuffer to the given position
     */
void move_cursor(int x, int y)
{
    if (y < 0 || y >= NUM_COLS)
    {
        return;
    }
    if (x < 0 || x >= NUM_ROWS)
    {
        return;
    }
    uint16_t pos = x * NUM_COLS + y;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    cursor_ptr->x = x;
    cursor_ptr->y = y;
}

int write(char *buf)
{
    print_strAt(buf, cursor_ptr->x, cursor_ptr->y);
    for (size_t i = 0; 1; i++)
    {
        char character = (uint8_t)buf[i];
        if (character == '\0')
        {
            break;
        }
        if (character == '\n')
        {
            if (cursor_ptr->x < NUM_ROWS - 1)
            {
                cursor_ptr->x=cursor_ptr->x+1;
            }
            cursor_ptr->y = 0;
        }
        else
        {
            cursor_ptr->y=cursor_ptr->y + 1;
        }
        if (cursor_ptr->y >= NUM_COLS)
        {
            cursor_ptr->x = cursor_ptr->x+1;
            if(cursor_ptr->x >= NUM_ROWS)
            {
                cursor_ptr->x = NUM_ROWS - 1;
            }
            cursor_ptr->y = 0;
        }
    }
    
    move_cursor(cursor_ptr->x, cursor_ptr->y);
}