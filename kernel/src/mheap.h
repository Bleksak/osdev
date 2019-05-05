#pragma once

#include <stddef.h>


WARN_UNUSED_RESULT void* malloc(size_t size);
WARN_UNUSED_RESULT void* calloc(size_t count, size_t size);
WARN_UNUSED_RESULT void* realloc(void* base, size_t new_size);
void  free(void* addr);
