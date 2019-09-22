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

    struct ProcessorLocalAPIC* cpu_apics[8];
    struct IOAPIC* io_apics[8];
    struct InterruptSourceOverride* interrupt_source_override[8];
    struct NonMaskableInterrupts* non_maskable_interrupts[8];
};

void apic_init(struct MADT_SDT* madt);
