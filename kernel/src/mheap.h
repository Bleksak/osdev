#pragma once

#include <stddef.h>


void* malloc(size_t size);
void* calloc(size_t count, size_t size);
void* realloc(void* base, size_t new_size);
void  free(void* addr);
