#pragma once

#include "std.h"

#include "attributes.h"

#define MAX_ALLOC_COUNT 1048576
#define MAX_SUPERPAGE 32
#define MAX_PAGE 32768

size_t page_get_value(size_t page);

WARN_UNUSED_RESULT void* palloc(const size_t count);
void pfree(void* addr, size_t count);
