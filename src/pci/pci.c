#include "pci.h"
#include <io.h>
#include <console.h>
#include <os.h>

enum PCI_PORTS {
    PCI_PORT_ADDRESS_SET = 0xCF8,
    PCI_PORT_ADDRESS_READ = 0xCFC,
};

static void pci_check_bus(uint8_t bus);

// returns 0xFFFF on a non existant device
uint32_t pci_config_read_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t lbus = (uint32_t) bus;
    uint32_t ldevice = (uint32_t) device;
    uint32_t lfunc = (uint32_t) function;
    uint32_t loffset = (uint32_t) offset;

    uint32_t address = (uint32_t)((lbus << 16) | (ldevice << 11) | (lfunc << 8) | (loffset & 0xFC) | 0x80000000);
    outl(PCI_PORT_ADDRESS_SET, address);

    return inl(PCI_PORT_ADDRESS_READ) >> ((offset & 2) * 8);
}

uint16_t pci_config_read_word (uint8_t bus, uint8_t device, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)device;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    outl(0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

uint32_t pci_get_vendor(uint8_t bus, uint8_t device, uint8_t function) {
    return pci_config_read_word(bus, device, function, 0);
}

uint32_t pci_get_header_type(uint8_t bus, uint8_t device, uint8_t function) {
    return (pci_config_read_dword(bus, device, function, 0x0C) >> 16) & 0xFF;
}

uint32_t pci_get_base_class(uint8_t bus, uint8_t device, uint8_t function) {
    return pci_config_read_dword(bus, device, function, 0x8) >> 24;
}

uint32_t pci_get_subclass(uint8_t bus, uint8_t device, uint8_t function) {
    return (pci_config_read_dword(bus, device, function, 0x8) >> 16) & 0xFF;
}

uint32_t pci_get_secondary_bus(uint8_t bus, uint8_t device, uint8_t function) {
    return (pci_config_read_dword(bus, device, function, 0x18) >> 8) & 0xFF;
}

static void pci_check_function(uint8_t bus, uint8_t device, uint8_t function) {
    uint8_t base_class = pci_get_base_class(bus, device, function);
    uint8_t subclass = pci_get_subclass(bus, device, function);

    

    printf("Found function on bus %d\n", bus);
    printf("Base class = %x\nSubclass = %x\n", base_class, subclass);


    if((base_class == 0x06) && (subclass == 0x04)) {
        printf("Checking bus %d\n", pci_get_secondary_bus(bus, device, function));
        pci_check_bus(pci_get_secondary_bus(bus, device, function));
    }
}

static void pci_check_device(uint8_t bus, uint8_t device) {
    uint32_t vendor = pci_get_vendor(bus, device, 0);
    if(vendor == 0xFFFF) {
        return;
    }

    pci_check_function(bus, device, 0);

    uint8_t header_type = pci_get_header_type(bus, device, 0);

    if(header_type & 0x80) {
        // multifunction device, check remaining functions

        for(uint8_t function = 1; function < 8; ++function) {
            if(pci_get_vendor(bus, device, function) != 0xFFFF) {
                pci_check_function(bus, device, function);
            }
        }
    }
}

static void pci_check_bus(uint8_t bus) {
    for(uint8_t device = 0; device < 32; ++device) {
        pci_check_device(bus, device);
    }
}

void pci_init(void) {
    for(uint32_t bus = 0; bus < 256; ++bus) {
        for(uint8_t device = 0; device < 32; ++device) {
            pci_check_device(bus, device);
        }
    }
}
