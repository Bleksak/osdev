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
    uint8_t ACPIProcessorID;
    uint8_t APICID;
    uint32_t Flags;
} PACKED;

struct IOAPICEntry { // Entry type 1
    uint8_t IOAPICID;
    uint8_t Reserved;
    uint32_t IOAPICAddress;
    uint32_t GlobalSystemInterruptBase;
} PACKED;

struct InterruptOverrideEntry {
    uint8_t BusSource;
    uint8_t IRQSource;
    uint32_t GlobalSystemInterrupt;
    uint16_t Flags;
} PACKED;

struct NMIEntry {
    uint8_t ACPIProcessorID; // 0xFF => all CPUs
    uint16_t Flags;
    uint8_t LINT; // 0 or 1
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

    union {
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


bool madt_parse(const struct SDT* header);
