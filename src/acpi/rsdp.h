#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../attributes.h"

struct _RsdpV1
{
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
} PACKED;

struct _RsdpV2
{
    struct _RsdpV1 v1;
    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];
} PACKED;

union RSDP
{
    struct _RsdpV1 v1;
    struct _RsdpV2 v2;
} PACKED;

union RSDP* RsdpInit(void);