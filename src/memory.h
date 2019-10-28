#pragma once
#include "std.h"


void memset(void* restrict dest, int ch, size_t len);
void memcpy(void* restrict dest, void* restrict src, size_t len);
bool memcmp(void* restrict dest, void* restrict src, size_t len);

void memset_classic(void* restrict dest, int c, size_t len);
