#include "bda.h"
#include "paging.h"

struct BiosDataArea* BDA = (struct BiosDataArea*)0x400;

void InitBiosDataArea()
{
    
}

struct BiosDataArea* getBDA()
{
    return BDA;
}