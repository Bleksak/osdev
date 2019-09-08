#pragma once

#include "disk/ata.h"
#include "disk/partition.h"

struct OS {
    Drive* drives;
};

typedef struct OS OS;

extern OS os;