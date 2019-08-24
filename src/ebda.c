#include "ebda.h"
#include "ebda.h"
#include "bda.h"
#include "assert.h"
#include "paging.h"


const struct ExtendedBiosDataArea* EBDA = 0;

void ExtendedBDAInit(const uintptr_t EBDAptr)
{
    const uintptr_t currentVirtualMemoryOffset = getCurrentVirtualMemoryOffset();

    const ptrdiff_t offset = map_page(EBDAptr, currentVirtualMemoryOffset, Present);
    setCurrentVirtualMemoryOffset(currentVirtualMemoryOffset + 0x1000);

    EBDA = (struct ExtendedBiosDataArea*) currentVirtualMemoryOffset + offset;

    assert(EBDA->ebda_size == 1 || EBDA->ebda_size == 2);
}

const struct ExtendedBiosDataArea* getEBDA(void)
{
    return EBDA;
}
