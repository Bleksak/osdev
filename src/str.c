#include "str.h"

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

bool str_cmp(const char* str1, const char* str2) {
    for(size_t i = 0; str1[i] && str2[i]; ++i) {
        if(str1[i] != str2[i]) {
            return false;
        }
    }

    return true;
}

void strrev(char* str) {
    for(size_t i = 0, j = strlen(str) - 1; i < j; ++i, --j) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
    }
}

int itoa(int number, unsigned int base, char* buffer) {
    
    if(number == 0) {
        buffer[0] = 0x30;
        buffer[1] = 0;
        return 1;
    }
    
    int i = 0;

    while(number) {
        buffer[i++] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[number % base];
        number /= base;
    }

    buffer[i] = 0;

    strrev(buffer);
    
    return i;
}

int ltoa(long long number, long long base, char* buffer) {
    int i = 0;

    while(number) {
        buffer[i++] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[number % base];
        number /= base;
    }

    buffer[i] = 0;

    strrev(buffer);
    
    return i;
}
