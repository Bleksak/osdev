#pragma once
#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char* str);
int strncmp(const char* str1, const char* str2, size_t len);
void strrev(char* str);

int itoa(int number, unsigned int base, char* buffer);
int ltoa(long long number, long long base, char* buffer);
bool str_cmp(const char* str1, const char* str2);
