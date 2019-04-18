#pragma once

#include "size_t.h"
#include <stdint.h>

__attribute__((const)) uintptr_t align(uintptr_t x, size_t alignment);