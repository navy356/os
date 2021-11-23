#include "string.h"

void *memset(void *str, int c, size_t n)
{
    int i;
    unsigned char *p = str;
    i = 0;
    while (n > 0)
    {
        *p = c;
        p++;
        n--;
    }
    return (str);
}