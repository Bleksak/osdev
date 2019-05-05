#include "ebda.h"
#include "ebda.h"
#include "bda.h"
#include "assert.h"
#include "paging.h"


const struct ExtendedBiosDataArea* EBDA = 0;

void ExtendedBDAInit(const uintptr_t EBDAptr)
{
    if(EBDAptr < MB)
    {
        uintptr_t ptr = (uintptr_t)MEM_ZERO_MAP;
        EBDA = (struct ExtendedBiosDataArea*) ptr + EBDAptr;
    }
    else
    {
        const uintptr_t currentVirtualMemoryOffset = getCurrentVirtualMemoryOffset();

        map_page(EBDAptr, currentVirtualMemoryOffset, Present);
        setCurrentVirtualMemoryOffset(currentVirtualMemoryOffset + 0x1000);

        const ptrdiff_t offset = EBDAptr & 0x0fff;

        EBDA = (struct ExtendedBiosDataArea*) currentVirtualMemoryOffset + 0x1000 + offset;
    }

    assert(EBDA->ebda_size == 1 || EBDA->ebda_size == 2);
}

const struct ExtendedBiosDataArea* getEBDA()
{
    return EBDA;
}