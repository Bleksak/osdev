#pragma once
#include <stdint.h>

#include <vector.h>

#ifndef bit_MSR
#define bit_MSR 0x00000020
#endif

#ifndef bit_APIC
#define bit_APIC 0x00000200
#endif

#define IA32_APIC_BASE_MSR        0x1B
#define IA32_APIC_BASE_MSR_BSP    0x100
#define IA32_APIC_BASE_MSR_X2APIC 0x400
#define IA32_APIC_BASE_MSR_ENABLE 0x800

#include "../acpi/madt.h"

Vector(LAPIC, LAPICEntry);
Vector(IOAPIC, IOAPICEntry);
Vector(InterruptOverride, InterruptOverrideEntry);
Vector(NMI, NMIEntry);

struct APIC {
    uintptr_t lapic_base;
    uint32_t flags;

    LAPIC lapic;
    IOAPIC ioapic;
    InterruptOverride interrupt_override;
    NMI nmi;
};

void lapic_enable(void);
void lapic_eoi(void);
