#pragma once
#include "../std.h"
#include "acpi.h"
#include "../attributes.h"


enum MADT_Entries {
    MADT_LAPIC,
    MADT_IOAPIC,
    MADT_INTERRUPT_OVERRIDE,
    MADT_NMI = 4,
    MADT_LAPIC_OVERRIDE = 5,
};

struct LAPICEntry { // Entry type 0 
    uint8_t acpi_id;
    uint8_t apic_id;
    uint32_t flags;
} PACKED;

struct IOAPICEntry { // Entry type 1
    uint8_t id;
    uint8_t reserved;
    uint32_t ioapic_address;
    uint32_t global_int;
} PACKED;

struct InterruptOverrideEntry {
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_system_interrupt;
    uint16_t flags;
} PACKED;

struct NMIEntry {
    uint8_t acpi_cpu; // 0xFF => all CPUs
    uint16_t flags;
    uint8_t lint; // 0 or 1
} PACKED;

struct LAPICOverride  {
    uint16_t reserved;
    uint64_t address;
} PACKED;

typedef struct LAPICEntry LAPICEntry;
typedef struct IOAPICEntry IOAPICEntry;
typedef struct InterruptOverrideEntry InterruptOverrideEntry;
typedef struct NMIEntry NMIEntry;
typedef struct LAPICOverride LAPICOverride;

struct MADTEntry {
    const uint8_t type;
    const uint8_t length;

    const union {
        const LAPICEntry lapic;
        const IOAPICEntry ioapic;
        const InterruptOverrideEntry interrupt_override;
        const NMIEntry nmi;
        const LAPICOverride lapic_override;
    };
} PACKED;

typedef struct MADTEntry MADTEntry;

struct MADT {
    const SDT sdt;
    const uint32_t lapic_addr;
    const uint32_t flags;
    const MADTEntry entries;
} PACKED;

typedef struct MADT MADT;


void madt_parse(const SDT* header);
