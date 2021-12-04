#include "string.h"
#include "kheap.h"
#include "print.h"
#include "io.h"

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

int strlen(char *str)
{
    int l = 0;
    while (str[l] != '\0')
    {
        l++;
    }
    return l;
}

char *strdup(char *str)
{
    int l = strlen(str);
    char *dup = (char *)malloc(sizeof(char) * (l + 1), 0);
    for (int i = 0; i < l; i++)
    {
        dup[i] = str[i];
    }
    dup[l] = '\0';
    return dup;
}

int strcmp(char *str1, char *str2)
{
    int l1 = strlen(str1);
    int l2 = strlen(str2);

    if (l1 != l2)
    {
        return 1;
    }

    for (int i = 0; i < l1; i++)
    {
        if (str1[i] != str2[i])
        {
            return 1;
        }
    }

    return 0;
}