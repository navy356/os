#pragma once
#include <stdint.h>

struct Cursor
{
    int x;
    int y;
};

#define MAX_BUFFER_SIZE 2000
char inbuf[MAX_BUFFER_SIZE];
void move_cursor(int,int);
int write(char *buf);
void init_io();
void keyboard_driver(char scanCode);
int controlKey(uint16_t c, int keydown);
char getc();
int writec(char c);
void gotInput(char c);
char sendInput();
char getc();