#include "string.h"

size_t strlen(const char* str) {
    size_t i = 0;
    while(str[i]) 
        i++;
    return i;
}

int strncmp(const char* str1, const char* str2, size_t len) {
    int sum = 0;

    for(size_t i = 0; i < len; ++i) {
        sum += str1[i] - str2[i];
    }

    return sum;
}

void strrev(char* str) {
    for(size_t i = 0, j = strlen(str) - 1; i < j; ++i, --j) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
    }
}
