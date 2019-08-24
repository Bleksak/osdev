#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

size_t strlen(const char* str);
int strncmp(const char* str1, const char* str2, size_t len);
void strrev(char* str);
