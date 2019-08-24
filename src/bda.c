#include "bda.h"
#include "ebda.h"
#include "paging.h"

#include "console.h"

#define BDA_OFFSET 0

const struct BiosDataArea* BDA;

void BiosDataAreaInit(void)
{
    const uintptr_t virtual_offset = getCurrentVirtualMemoryOffset();
    map_page(0, virtual_offset, Present);
    setCurrentVirtualMemoryOffset(virtual_offset + 0x1000);

    BDA = (const struct BiosDataArea*)virtual_offset;

    ExtendedBDAInit(BDA->ExtendedBDAPtr * 0x10);
}

const struct BiosDataArea* getBDA(void)
{
    return BDA;
}