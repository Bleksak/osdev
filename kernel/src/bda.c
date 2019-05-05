#include "bda.h"
#include "ebda.h"
#include "paging.h"

#include "console.h"

#define BDA_OFFSET 0

const struct BiosDataArea* BDA = (struct BiosDataArea*) (VIRTUAL_MEMORY_START + BDA_OFFSET);

void BiosDataAreaInit()
{
    map_page(0, VIRTUAL_MEMORY_START, Present);
    ExtendedBDAInit(BDA->ExtendedBDAPtr * 0x10);
}

const struct BiosDataArea* getBDA()
{
    return BDA;
}