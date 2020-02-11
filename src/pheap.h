#pragma once
#include <stddef.h>

#define MAX_ALLOC_COUNT 1048576
#define MAX_SUPERPAGE 32
#define MAX_PAGE 32768
#define PAGE_SIZE 0x1000

size_t page_get_value(size_t page);

__attribute__((warn_unused_result)) void* palloc(const size_t count);
__attribute__((warn_unused_result)) void* repalloc(void* addr, const size_t oldcount, const size_t newcount);

void pfree(void* addr, size_t count);
