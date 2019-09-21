#include "pci.h"
#include "../io.h"

#include "../console.h"

enum PCI_PORTS {
    PCI_PORT_ADDRESS_SET = 0xCF8,
    PCI_PORT_ADDRESS_READ = 0xCFC,
};

// returns 0xFFFF on a non existant device
uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset) {
    uint32_t lbus = (uint32_t) bus;
    uint32_t ldevice = (uint32_t) device;
    uint32_t lfunc = (uint32_t) func;
    uint32_t loffset = (uint32_t) offset;

    uint32_t address = (uint32_t)((lbus << 16) | (ldevice << 11) | (lfunc << 8) | (loffset & 0xFC) | 0x80000000);
    outl(PCI_PORT_ADDRESS_SET, address);

    return (uint16_t)((inl(PCI_PORT_ADDRESS_READ) >> ((offset & 2) * 8)) & 0xFFFF);
}

uint16_t pci_get_vendor(uint8_t bus, uint8_t device, uint8_t function) {
    return pci_config_read_word(bus, device, function, 0);
}

uint16_t pci_get_header_type(uint8_t bus, uint8_t device, uint8_t function) {
    return pci_config_read_word(bus, device, function, 0x24);
}

bool pci_check_device(uint8_t bus, uint8_t device)
{
    uint8_t function = 0;
    uint16_t vendorID = pci_get_vendor(bus, device, function);
    if(vendorID == 0xFFFF)
        return false;
    


    return true;
}

// uint16_t pci_check_vendor(uint8_t bus, uint8_t device)
// {

// }