#pragma once

#include "disk/ata.h"
#include "acpi/acpi.h"
#include "acpi/apic.h"
#include "pci/pci.h"
#include "cpu/cpuid.h"

struct OS {
    Drive* drives;
    struct APIC apic;
    struct CPUID cpuid;
    struct PCI pci[256*32];
};

typedef struct OS OS;

extern OS os;
