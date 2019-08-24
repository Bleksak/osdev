#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


void memset(void* restrict dest, uint8_t ch, size_t len);
void memcpy(void* restrict dest, void* restrict src, size_t len);
bool memcmp(void* restrict dest, void* restrict src, size_t len);