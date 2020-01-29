#pragma once
#include <stddef.h>

#define MAX_ALLOC_COUNT 1048576
#define MAX_SUPERPAGE 32
#define MAX_PAGE 32768

size_t page_get_value(size_t page);

__attribute__((warn_unused_result)) void* palloc(const size_t count);
void pfree(void* addr, size_t count);
