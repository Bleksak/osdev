#pragma once

#include "../std.h"

// there are 256 buses
// there are 32 devices on each bus
// there can be 8 functions per device
// offset is the info that you want to receive

uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pci_get_header_type(uint8_t bus, uint8_t device, uint8_t function);

void pci_init(void);

struct PCI {
    uint16_t device_id;
    uint16_t vendor_id;
    uint16_t status;
    uint16_t command;
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
    uint8_t revision;
    uint8_t BIST;
    uint8_t header_type;
    uint8_t latency_timer;
    uint8_t cache_line_size;
    uint32_t BAR0;
    uint32_t BAR1;
    uint32_t BAR2;
    uint32_t BAR3;
    uint32_t BAR4;
    uint32_t BAR5;
    uint32_t cardbus_cis_pointer;
    uint16_t subsystem_id;
    uint16_t subsystem_vendor_id;
    uint32_t expansion_rom_base_address;
    uint8_t capabilities_ptr;
    uint8_t max_latency;
    uint8_t min_grant;
    uint8_t interrupt_pin;
    uint8_t interrupt_line;
};