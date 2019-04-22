#include "ebda.h"
#include "ebda.h"
#include "bda.h"
#include "assert.h"

struct ExtendedBiosDataArea* EBDA = 0;

void ExtendedBDAInit()
{
    EBDA = (struct ExtendedBiosDataArea*) ((uintptr_t)(getBDA()->ExtendedBDAPtr * 0x10));
    //assert(EBDA->ebda_size != 0 && EBDA->ebda_size != 1);
}

struct ExtendedBiosDataArea* getEBDA()
{
    return EBDA;
}