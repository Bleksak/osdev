#pragma once

#include "../std.h"

// there are 256 buses
// there are 32 devices on each bus
// there can be 8 functions per device
// offset is the info that you want to receive

uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

uint16_t pci_get_header_type(uint8_t bus, uint8_t device, uint8_t function);
