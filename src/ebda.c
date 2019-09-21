#include "ebda.h"
#include "ebda.h"
#include "bda.h"
#include "assert.h"
#include "paging.h"

const struct ExtendedBiosDataArea* ebda = 0;

void ebda_init(const uintptr_t ebda_ptr)
{
    const uintptr_t currentVirtualMemoryOffset = mem_offset_get();

    const ptrdiff_t offset = map_page(ebda_ptr, currentVirtualMemoryOffset, Present);
    mem_offset_set(currentVirtualMemoryOffset + 0x1000);

    ebda = (struct ExtendedBiosDataArea*) currentVirtualMemoryOffset + offset;

    assert(ebda->ebda_size == 1 || ebda->ebda_size == 2);
}

const struct ExtendedBiosDataArea* ebda_get(void) {
    return ebda;
}
