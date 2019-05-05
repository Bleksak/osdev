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

bool doChecksum(uint8_t* ptr, size_t len);
bool AcpiInit();