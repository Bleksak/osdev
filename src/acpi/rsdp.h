#pragma once

#include "../std.h"

struct _RsdpV1 {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct _RsdpV2 {
    struct _RsdpV1 v1;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t ext_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

union RSDP {
    struct _RsdpV1 v1;
    struct _RsdpV2 v2;
} __attribute__((packed));

union RSDP* rsdp_init(void);
