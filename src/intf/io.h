#pragma once
#include <stdint.h>

struct Cursor
{
    int x;
    int y;
};

void move_cursor(int,int);
int write(char *buf, unsigned int len);
void init_io();