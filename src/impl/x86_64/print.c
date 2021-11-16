#include "print.h"
#include "constants.h"
#include "math.h"

#define HEX_TO_STRING_MAX 11
struct Char
{
    uint8_t character;
    uint8_t color;
};

struct Char *buffer = (struct Char *)0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row)
{
    struct Char empty = (struct Char){
        character : ' ',
        color : color,
    };

    for (size_t col = 0; col < NUM_COLS; col++)
    {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_clear()
{
    for (size_t i = 0; i < NUM_ROWS; i++)
    {
        clear_row(i);
    }
}

void print_newline()
{
    col = 0;

    if (row < NUM_ROWS - 1)
    {
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++)
    {
        for (size_t col = 0; col < NUM_COLS; col++)
        {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row(NUM_ROWS - 1);
}

int print_newlineFor(int x, int y)
{
    if (x < NUM_ROWS - 1)
    {
        x++;
        return x;
    }

    for (size_t row = 1; row < NUM_ROWS; row++)
    {
        for (size_t col = 0; col < NUM_COLS; col++)
        {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row(NUM_ROWS - 1);
    return x;
}

void print_char(char character)
{
    if (character == '\n')
    {
        print_newline();
        return;
    }

    if (col > NUM_COLS)
    {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char){
        character : (uint8_t)character,
        color : color,
    };

    col++;
}

int print_charAt(char character, int x, int y)
{
    if (character == '\n')
    {
        x = print_newlineFor(x, y);
        return x;
    }

    buffer[y + NUM_COLS * x] = (struct Char){
        character : (uint8_t)character,
        color : color,
    };

    return x;
}

void print_str(char *str)
{
    for (size_t i = 0; 1; i++)
    {
        char character = (uint8_t)str[i];

        if (character == '\0')
        {
            return;
        }

        print_char(character);
    }
}

void print_strAt(char *str, int x, int y)
{
    if (x >= NUM_ROWS || x < 0)
    {
        return;
    }
    if (y >= NUM_COLS || y < 0)
    {
        return;
    }
    for (size_t i = 0; 1; i++)
    {
        char character = (uint8_t)str[i];

        if (character == '\0')
        {
            return;
        }

        x = print_charAt(character, x, y);

        y++;

        if (y >= NUM_COLS)
        {
            x = print_newlineFor(x, y);
            y = 0;
        }

        if (character == '\n')
        {
            y = 0;
        }
    }
}

void print_set_color(uint8_t foreground, uint8_t background)
{
    color = foreground + (background << 4);
}

char hexToStringOutput[HEX_TO_STRING_MAX];

char *hexToString(unsigned int num)
{
    int i;
    hexToStringOutput[0] = '0';
    hexToStringOutput[1] = 'x';
    int len = hexLenHelper(num)+1;
    char index[16] = {'0', '1', '2', '3', '4', '5','6','7','8','9','A','B','C','D','E', 'F'};
    for(i=len; num>0; i--)
    {
        hexToStringOutput[i] = index[(num & (0xF)) ];
        num = num>>4;
    }
    hexToStringOutput[len+1]=0;
    return hexToStringOutput;
}