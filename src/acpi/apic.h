#pragma once
#include "../std.h"
#include "madt.h"

struct APIC
{
    uintptr_t lapic_addr;
    uint32_t flags;

    uint8_t cpu_count;
    uint8_t ioapic_count;
    uint8_t interrupt_source_override_count;
    uint8_t non_maskable_interrupts_count;

    const struct ProcessorLocalAPIC* cpu_apics[8];
    const struct IOAPIC* io_apics[8];
    const struct InterruptSourceOverride* interrupt_source_override[8];
    const struct NonMaskableInterrupts* non_maskable_interrupts[8];
};

void apic_init(const struct MADT_SDT* madt);
