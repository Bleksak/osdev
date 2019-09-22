#pragma once

#include "disk/ata.h"
#include "acpi/acpi.h"

struct OS {
    Drive* drives;
    // struct {
        
    // } acpi;
};

typedef struct OS OS;

extern OS os;
