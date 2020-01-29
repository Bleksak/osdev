#pragma once

#include "disk/ata.h"
#include "acpi/acpi.h"
#include "smp/apic.h"
#include "pci/pci.h"
#include "cpu/cpuid.h"
#include "interrupt/interrupt.h"

struct OS {
    Drive* drives;
    struct APIC apic;
    struct CPUID cpuid;
    struct Interrupts interrupt;
    struct PCI pci[256*32];
};

extern struct OS os;

void os_init(void);
