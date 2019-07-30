#include "align.h"


inline uintptr_t align(uintptr_t x, size_t alignment)
{
    return ((x + (alignment - 1)) & -alignment);
}