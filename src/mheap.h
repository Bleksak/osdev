#pragma once

#include <stdint.h>
#include <stddef.h>

__attribute__((warn_unused_result)) void* malloc(size_t size);
__attribute__((warn_unused_result)) void* calloc(size_t count, size_t size);
__attribute__((warn_unused_result)) void* realloc(void* base, size_t new_size);
void  free(void* addr);

size_t heap_get_free_mem(void);
void heap_init(void);
