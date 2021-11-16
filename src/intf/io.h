#pragma once
#include <stdint.h>

struct Cursor
{
    int x;
    int y;
};

void move_cursor(int,int);
int write(char *buf);
void init_io();