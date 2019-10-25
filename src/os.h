#pragma once

#include "disk/ata.h"
#include "acpi/acpi.h"
#include "acpi/apic.h"
#include "pci/pci.h"


struct OS {
    Drive* drives;
    struct APIC apic;
    struct PCI pci[256*32];
};

typedef struct OS OS;

extern OS os;
