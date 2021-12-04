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

int inbuf_ptr;
struct Cursor cursor;

struct Cursor *cursor_ptr;

void init_io()
{
    cursor_ptr = &cursor;
    cursor_ptr->x = 0;
    cursor_ptr->y = 0;
    inbuf_ptr = -1;
}

void gotInput(char c)
{
    if (inbuf_ptr >= (MAX_BUFFER_SIZE - 1))
    {
        return;
    }
    else
    {
        inbuf_ptr++;
        inbuf[inbuf_ptr] = c;
    }
    writec(c);
}

char sendInput()
{
    if(inbuf_ptr<0)
    {
        return '\0';
    }
    if (inbuf_ptr >= 0)
    {
        char c = inbuf[inbuf_ptr];
        inbuf_ptr--;
        return c;
    }
    return '\0';
}

char getc()
{
    char c = sendInput();
    while (c == '\0')
    {
        c = sendInput();
    }
    return c;
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
                cursor_ptr->x = cursor_ptr->x + 1;
            }
            cursor_ptr->y = 0;
        }
        else
        {
            cursor_ptr->y = cursor_ptr->y + 1;
        }
        if (cursor_ptr->y >= NUM_COLS)
        {
            cursor_ptr->x = cursor_ptr->x + 1;
            if (cursor_ptr->x >= NUM_ROWS)
            {
                cursor_ptr->x = NUM_ROWS - 1;
            }
            cursor_ptr->y = 0;
        }
    }

    move_cursor(cursor_ptr->x, cursor_ptr->y);
}

int writec(char c)
{
    print_charAt(c, cursor_ptr->x, cursor_ptr->y);
    if (cursor_ptr->x == 24 && cursor_ptr->y == 79)
    {
        print_newlineFor(cursor_ptr->x, cursor_ptr->y);
    }
    if (c == '\n')
    {
        if (cursor_ptr->x < NUM_ROWS - 1)
        {
            cursor_ptr->x = cursor_ptr->x + 1;
        }
        cursor_ptr->y = 0;
    }
    else
    {
        cursor_ptr->y = cursor_ptr->y + 1;
    }
    if (cursor_ptr->y >= NUM_COLS)
    {
        cursor_ptr->x = cursor_ptr->x + 1;
        if (cursor_ptr->x >= NUM_ROWS)
        {
            cursor_ptr->x = NUM_ROWS - 1;
        }
        cursor_ptr->y = 0;
    }

    move_cursor(cursor_ptr->x, cursor_ptr->y);
}

void keyboard_driver(char scanCode)
{
    int keydown = 1;
    if (scanCode & 0x80)
    {
        keydown = 0; //keyup
        scanCode = scanCode & 0x80;
    }

    uint16_t c = _kkybrd_scancode_std[scanCode];
    if (controlKey(c, keydown))
    {
        return;
    }

    if (keydown && (c >> 8) == 0)
    {
        if(c==KEY_RETURN)
        {
            gotInput('\n');
        }
        else
        {
            gotInput((char)c);
        }
        //print_char((char)c);
    }
}

int controlKey(uint16_t c, int keydown)
{
    switch (c)
    {
    case KEY_LCTRL:
    case KEY_RCTRL:
        if (keydown)
        {
            mod_keys |= MOD_CTRL;
        }
        else
        {
            mod_keys &= MOD_CTRL;
        }
        return 1;
    case KEY_CAPSLOCK:
        if (!keydown)
        {
            mod_keys = ~mod_keys;
        }
        return 1;
    case KEY_LSHIFT:
    case KEY_RSHIFT:
        if (keydown)
        {
            mod_keys |= MOD_SHIFT;
        }
        else
        {
            mod_keys &= MOD_SHIFT;
        }
        return 1;
    default:
        return 0;
    }
}

uint8_t kybrd_ctrl_read_status()
{
    return inb(KYBRD_CTRL_STATS_REG);
}

//send command byte to controller
void kybrd_ctrl_send_cmd(uint8_t cmd)
{
    //wait for controller input buffer to be clear
    while (1)
        if ((kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
            break;
    outb(KYBRD_CTRL_CMD_REG, cmd);
}

//read encoder buffer
uint8_t kybrd_enc_read_buf()
{
    return inb(KYBRD_ENC_INPUT_BUF);
}

//write command byte to encoder
void kybrd_enc_send_cmd(uint8_t cmd)
{
    //wait for controller input buffer to be clear
    while (1)
        if ((kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
            break;
    // send command byte to encoder
    outb(KYBRD_ENC_CMD_REG, cmd);
}