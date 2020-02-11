#pragma once

#include <stdint.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


void memset(void* restrict dest, int ch, size_t len);
__attribute__((warn_unused_result)) void* memcpy(void* restrict dest, void* restrict src, size_t len);
bool memcmp(void* restrict dest, void* restrict src, size_t len);

void memset_classic(void* restrict dest, int c, size_t len);
