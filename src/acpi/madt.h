#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "acpi.h"
#include "../attributes.h"


enum MADT_Entries
{
    ProcessorLocalAPIC,
    IOAPIC,
    InterruptSourceOverride,
    NonMaskableInterrupts = 4,
    LocalAPICAddressOverride = 5,
};

struct ProcessorLocalAPIC // Entry type 0 
{
    uint8_t ACPIProcessorID;
    uint8_t APICID;
    uint32_t Flags;
} PACKED;

struct IOAPIC // Entry type 1
{
    uint8_t IOAPICID;
    uint8_t Reserved;
    uint32_t IOAPICAddress;
    uint32_t GlobalSystemInterruptBase;
} PACKED;

struct InterruptSourceOverride
{
    uint8_t BusSource;
    uint8_t IRQSource;
    uint32_t GlobalSystemInterrupt;
    uint16_t Flags;
} PACKED;

struct NonMaskableInterrupts
{
    uint8_t ACPIProcessorID; // 0xFF => all CPUs
    uint16_t Flags;
    uint8_t LINT; // 0 or 1
} PACKED;

struct LocalAPICAddressOverride 
{
    uint16_t reserved;
    uint64_t address;
} PACKED;

struct MADT_Entry
{
    uint8_t Type;
    uint8_t Length;

    union
    {
        struct ProcessorLocalAPIC ProcessorLocalAPIC;
        struct IOAPIC IOAPIC;
        struct InterruptSourceOverride InterruptSourceOverride;
        struct NonMaskableInterrupts NonMaskableInterrupts;
        struct LocalAPICAddressOverride LocalAPICAddressOverride;
    };
} PACKED;

struct MADT_SDT
{
    struct SDT_Header SDT;
    uint32_t LocalApicAddress;
    uint32_t Flags;

    struct MADT_Entry entries;
} PACKED;

bool initMADT(struct MADT_SDT* header);