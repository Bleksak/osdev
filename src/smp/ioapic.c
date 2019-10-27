#include "ioapic.h"
#include "../isr.h"
#include "../paging.h"
#include "../console.h"

// Memory offsets
#define IOAPIC_IOREGSEL 0x00
#define IOAPIC_IOREGWIN 0x10

// Registers for memory
#define IOAPIC_REG_IOAPICID  0
#define IOAPIC_REG_IOAPICVER 1
#define IOAPIC_REG_IOAPICARB 2
#define IOAPIC_REG_IOREDTBL  0x10

// Different delivery modes
#define IOAPIC_DELIVERY_MODE_FIX    0
#define IOAPIC_DELIVERY_MODE_LOW    1
#define IOAPIC_DELIVERY_MODE_SMI    2
#define IOAPIC_DELIVERY_MODE_NMI    4
#define IOAPIC_DELIVERY_MODE_INIT   5
#define IOAPIC_DELIVERY_MODE_EXTINT 6

// Different destination modes
#define IOAPIC_DEST_PHYSICAL 0
#define IOAPIC_DEST_LOGICAL  1

// Different pin polarities
#define IOAPIC_PIN_HIGH 0
#define IOAPIC_PIN_LOW  1

// Different trigger modes
#define IOAPIC_TRIGGER_EDGE  0
#define IOAPIC_TRIGGER_LEVEL 1

typedef struct {
    uint64_t vector           : 8;
    uint64_t delivery_mode    : 3;
    uint64_t destination_mode : 1;
    uint64_t delivery_status  : 1;
    uint64_t pin_polarity     : 1;
    uint64_t remote_irr       : 1;
    uint64_t trigger_mode     : 1;
    uint64_t mask             : 1;
    uint64_t reserved1        : 32;
    uint64_t  reserved2        : 7;
    uint64_t destination      : 8;
} __attribute__((packed, aligned(4))) ioapic_redirect_entry_t;

uintptr_t ioapic_base;

// Write a 32bit value to a IO APIC register
void ioapic_register_writel(uint8_t offset, uint32_t val) {
    *(volatile uint32_t*) (ioapic_base + IOAPIC_IOREGSEL) = offset;
    *(volatile uint32_t*) (ioapic_base + IOAPIC_IOREGWIN) = val;
}

// Read a 32bit value from a IO apic register
uint32_t ioapic_register_readl(uint8_t offset) {
    *(volatile uint32_t*) (ioapic_base + IOAPIC_IOREGSEL) = offset;
    return *(volatile uint32_t*) (ioapic_base + IOAPIC_IOREGWIN);
}

// Get the I/O APIC ID
uint32_t ioapic_get_id() {
    return (ioapic_register_readl(IOAPIC_REG_IOAPICID) >> 24) & 0xF;
}

// Get the I/O APIC version
uint32_t ioapic_get_version() {
    return ioapic_register_readl(IOAPIC_REG_IOAPICVER) & 0xFF;
}

// Get the I/O APIC supported IRQ count
uint32_t ioapic_get_irqs() {
    return ((ioapic_register_readl(IOAPIC_REG_IOAPICVER) >> 16) & 0xFF) + 1;
}

// Get the redirect entry of an IRQ
ioapic_redirect_entry_t ioapic_get_redirect_entry(uint8_t irq) {
    uint32_t reg = IOAPIC_REG_IOREDTBL + (2 * irq);

    uint64_t data = (uint64_t) ioapic_register_readl(reg) |
                   ((uint64_t) ioapic_register_readl(reg + 1) << 32);

    return *(ioapic_redirect_entry_t*) &data;
}

// Set the redirect entry of an IRQ
void ioapic_set_redirect_entry(uint8_t irq, ioapic_redirect_entry_t redir) {
    uint32_t reg = IOAPIC_REG_IOREDTBL + (2 * irq);
    uint32_t* data = (uint32_t*) &redir;

    ioapic_register_writel(reg, data[0]);
    ioapic_register_writel(reg + 1, data[1]);
}

// Enable a specific interrupt
void ioapic_enable_irq(uint32_t irq, uint8_t vector) {
    ioapic_redirect_entry_t redir = ioapic_get_redirect_entry(irq);

    redir.vector = vector;
    redir.delivery_mode = IOAPIC_DELIVERY_MODE_FIX;
    redir.destination_mode = IOAPIC_DEST_PHYSICAL;
    redir.mask = 0;
    redir.destination = 0;

    ioapic_set_redirect_entry(irq, redir);
    printf("ioapic: mapping IRQ#%d to interrupt %d\n", irq, vector);
}

// Initialize the I/O APIC but disable everything
void ioapic_setup(uintptr_t base) {
    // Virtual page + page offset
    ioapic_base = base;

    printf("ioapic: ID: %d\n", ioapic_get_id());
    printf("ioapic: version: %x\n", ioapic_get_version());
    printf("ioapic: IRQ#: %d\n", ioapic_get_irqs());

    // Disable all interrupts
    for ( uint32_t i = 0; i < ioapic_get_irqs(); i++ ) {
        ioapic_redirect_entry_t redir = ioapic_get_redirect_entry(i);

        redir.mask = 1;

        ioapic_set_redirect_entry(i, redir);
    }

    printf("ioapic: all entries disabled.\n");
    pic_disable();
}
