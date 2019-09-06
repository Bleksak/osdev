#pragma once

#include "std.h"

#include "attributes.h"

WARN_UNUSED_RESULT void* palloc(size_t count);
void pfree(void* addr, size_t count);