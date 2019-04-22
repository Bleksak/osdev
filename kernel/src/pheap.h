#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void* palloc(size_t count);
void pfree(void* addr, size_t count);
