#pragma once

#include <stdbool.h>
#include <stddef.h>

unsigned int strlen(const char* str);
void strrev(char* const str);
bool strncmp(char* restrict str1, char* restrict str2, size_t len);