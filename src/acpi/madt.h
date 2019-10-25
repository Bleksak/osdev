#pragma once
#include "../std.h"
#include "acpi.h"
#include "../attributes.h"


enum MADT_Entries {
    ProcessorLocalAPIC,
    IOAPIC,
    InterruptSourceOverride,
    NonMaskableInterrupts = 4,
    LocalAPICAddressOverride = 5,
};

struct ProcessorLocalAPIC { // Entry type 0 
    uint8_t ACPIProcessorID;
    uint8_t APICID;
    uint32_t Flags;
} PACKED;

struct IOAPIC { // Entry type 1
    uint8_t IOAPICID;
    uint8_t Reserved;
    uint32_t IOAPICAddress;
    uint32_t GlobalSystemInterruptBase;
} PACKED;

struct InterruptSourceOverride {
    uint8_t BusSource;
    uint8_t IRQSource;
    uint32_t GlobalSystemInterrupt;
    uint16_t Flags;
} PACKED;

struct NonMaskableInterrupt {
    uint8_t ACPIProcessorID; // 0xFF => all CPUs
    uint16_t Flags;
    uint8_t LINT; // 0 or 1
} PACKED;

struct LocalAPICAddressOverride  {
    uint16_t reserved;
    uint64_t address;
} PACKED;

struct MADT_Entry {
    const uint8_t type;
    const uint8_t length;

    union {
        const struct ProcessorLocalAPIC lapic;
        const struct IOAPIC io_apic;
        const struct InterruptSourceOverride interrupt_source_override;
        const struct NonMaskableInterrupt non_maskable_interrupt;
        const struct LocalAPICAddressOverride lapic_addr_override;
    };
} PACKED;

struct MADT_SDT {
    const struct SDT sdt;
    const uint32_t lapic_addr;
    const uint32_t flags;

    const struct MADT_Entry entries;
} PACKED;

bool madt_parse(const struct SDT* header);
