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

bool strncmp(char* restrict str1, char* restrict str2, size_t len)
{
    for(size_t i = 0; i < len; ++i)
    {
        if(str1[i] != str2[i])
            return false;

        if(str1[i] == 0)
            return true;
    }

    return true;
}