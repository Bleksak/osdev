#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

WARN_UNUSED_RESULT void* palloc(size_t count);
void pfree(void* addr, size_t count);
