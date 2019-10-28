#pragma once

#include "disk/ata.h"
#include "acpi/acpi.h"
#include "smp/apic.h"
#include "pci/pci.h"
#include "cpu/cpuid.h"

struct OS {
    Drive* drives;
    struct APIC apic;
    struct CPUID cpuid;
    struct PCI pci[256*32];

    uint32_t irq_vectors[256];

};

typedef struct OS OS;

extern OS os;
