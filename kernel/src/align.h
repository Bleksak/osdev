#pragma once

#include <stddef.h>
#include <stdint.h>

__attribute__((const)) uintptr_t align(uintptr_t x, size_t alignment);