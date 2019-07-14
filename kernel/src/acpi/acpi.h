#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

struct SDT_Header
{
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
} PACKED;

enum ACPI_Capabilities
{
    ACPI_MADT = 1 << 0,
    ACPI_BERT = 1 << 1,
    ACPI_CPEP = 1 << 2,
    ACPI_DSDT = 1 << 3,
    ACPI_ECDT = 1 << 4,
    ACPI_EINJ = 1 << 5,
    ACPI_ERST = 1 << 6,
    ACPI_FADT = 1 << 7,
    ACPI_FACS = 1 << 8,
    ACPI_HEST = 1 << 9,
    ACPI_MSCT = 1 << 10,
    ACPI_MPST = 1 << 11,
    ACPI_OEMx = 1 << 12,
    ACPI_PMTT = 1 << 13,
    ACPI_PSDT = 1 << 14,
    ACPI_RASF = 1 << 15,
    ACPI_RSDT = 1 << 16,
    ACPI_SBST = 1 << 17,
    ACPI_SLIT = 1 << 18,
    ACPI_SRAT = 1 << 19,
    ACPI_SSDT = 1 << 20,
    ACPI_XSDT = 1 << 21,
};

uint32_t getAcpiCapabilities(void);

bool doChecksum(uint8_t* ptr, size_t len);
bool AcpiInit(void);