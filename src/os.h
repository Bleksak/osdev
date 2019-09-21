#pragma once

#include "disk/ata.h"

struct OS {
    Drive* drives;
};

typedef struct OS OS;

extern OS os;
