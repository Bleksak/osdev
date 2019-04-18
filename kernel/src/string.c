#include "string.h"

unsigned int strlen(const char* str)
{
    unsigned int i = 0;
    while(str[i]) ++i;
    return i;
}

void strrev(char* const str)
{
    unsigned int i = 0, j = strlen(str) - 1;
    char tmp;

    while(i<j)
    {
        tmp = str[i];
        str[i++] = str[j];
        str[j--] = tmp;
    }
}
