#include "align.h"

inline uintptr_t align(uintptr_t x, size_t alignment)
{
    size_t remainder = x % alignment;
    return (remainder) ? x + (alignment - remainder) : x;
}