#pragma once
#include "bda.h"
#include <stdint.h>

struct ExtendedBiosDataArea
{
    uint8_t ebda_size; // 1-> 1024B, 2 -> 2048B
} PACKED;

void ExtendedBDAInit(const uintptr_t EBDAptr);

const struct ExtendedBiosDataArea* getEBDA();