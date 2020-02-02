#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

uintptr_t align(uintptr_t x, size_t alignment);
bool aligncmp(uintptr_t a, uintptr_t b, size_t alignment);
