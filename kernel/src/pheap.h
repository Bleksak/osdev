#pragma once


#include <stdint.h>
#include <stdbool.h>
#include "size_t.h"

void* palloc(size_t count);
void pfree(void* addr, size_t count);
