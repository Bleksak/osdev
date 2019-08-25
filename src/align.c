#include "align.h"
#include "assert.h"

#include "cpu/ext/sse.h"

inline uintptr_t align(uintptr_t x, size_t alignment)
{
    assert(popcntl(alignment) == 1);

    return ((x + (alignment - 1)) & -alignment);
}
