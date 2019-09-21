#include "ioapic.h"
#include "apic.h"
#include "../console.h"

enum IOAPIC_REGISTERS {
    IOAPICID,
    IOAPICVER,
    IOAPICARB,
    IOREDTBL = 0x10,
    IOREDTBL2 = 0x11,
};

static void ioapic_write(const uintptr_t base, const uint8_t offset, const uint32_t value) {
    *(volatile uint32_t*)(base) = offset;
    *(volatile uint32_t*)(base + 0x10) = value;
}

static uint32_t ioapic_read(const uintptr_t base, const uint8_t offset) {
    *(volatile uint32_t*) (base) = offset;
    return *(volatile uint32_t*) (base + 0x10);
}

void ioapic_init(struct APIC* apic) {
    for(size_t i = 0; i < apic->ioapic_count; ++i) {
        
        const uintptr_t ioapic_address = apic->io_apics[i]->IOAPICAddress;

        const uint8_t ioapic_id = ioapic_read(ioapic_address, IOAPICID) >> 28;

        const uint32_t ioapicver = ioapic_read(ioapic_address, IOAPICVER);
        const uint8_t version = ioapicver & 0xF;
        const uint8_t max_redirection_entry = (ioapicver >> 16) & 0xFF;

        const uint8_t arb_id = ioapic_read(ioapic_address, IOAPICARB) >> 28;
        
        
    }
}