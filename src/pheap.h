#pragma once


#include <stdint.h>
#include <stdbool.h>
#include "stdlib/stddef.h"

#include "attributes.h"

WARN_UNUSED_RESULT void* palloc(size_t count);
void pfree(void* addr, size_t count);