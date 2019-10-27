#pragma once
#include "../std.h"

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

struct APIC {
    uintptr_t lapic_addr;
    uint32_t flags;

    uint8_t cpu_count;
    uint8_t ioapic_count;
    uint8_t interrupt_source_override_count;
    uint8_t non_maskable_interrupts_count;

    struct ProcessorLocalAPIC* cpu_apics;
    struct IOAPIC* io_apics;
    struct InterruptSourceOverride* interrupt_source_override;
    struct NonMaskableInterrupt* non_maskable_interrupts;
};

// void apic_init(const struct APIC* apic);
void lapic_enable(void);