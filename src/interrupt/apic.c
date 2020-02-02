#include "apic.h"
#include <paging.h>
#include <cpu/cr.h>
#include <console.h>
#include <interrupt.h>
#include <os.h>

extern void irq_empty_stub();

#define APIC_REG_ID                  0x0020
#define APIC_REG_VERSION             0x0030
#define APIC_REG_TASK_PRIORITY       0x0080
#define APIC_REG_END_OF_INTERRUPT    0x00B0
#define APIC_REG_LOGICAL_DESTINATION 0x00D0
#define APIC_REG_DESTINATION_FORMAT  0x00E0
#define APIC_REG_SPURIOUS_INTERRUPT  0x00F0
#define APIC_REG_INSERVICE_ROUTINE   0x0100
#define APIC_REG_ERROR_STATUS        0x0280
#define APIC_REG_INTERRUPT_CMD_LOW   0x0300
#define APIC_REG_INTERRUPT_CMD_HIGH  0x0310
#define APIC_REG_LVT_TIMER           0x0320
#define APIC_REG_LVT_PERF_COUNTER    0x0340
#define APIC_REG_LVT_LINT0           0x0350
#define APIC_REG_LVT_LINT1           0x0360
#define APIC_REG_LVT_ERROR           0x0370
#define APIC_TIMER_INITIAL_COUNT     0x0380
#define APIC_TIMER_CURRENT_COUNT     0x0390
#define APIC_TIMER_DIVIDER           0x03E0
#define APIC_ICR_CMD_INIT            0x0500
#define APIC_ICR_CMD_STARTUP         0x0600
#define APIC_ICR_FIXED               0x0000
#define APIC_ICR_LOWEST              0x0100
#define APIC_ICR_SMI                 0x0200
#define APIC_ICR_NMI                 0x0400
#define APIC_ICR_PHYSICAL            0x0000
#define APIC_ICR_LOGICAL             0x0800
#define APIC_ICR_IDLE                0x0000
#define APIC_ICR_SEND_PENDING        0x1000
#define APIC_ICR_DEASSERT            0x0000
#define APIC_ICR_ASSERT              0x4000
#define APIC_ICR_EDGE                0x0000
#define APIC_ICR_LEVEL               0x8000

// Return bool if the APIC is supported

bool lapic_is_x2apic() {
    const uint32_t flags = IA32_APIC_BASE_MSR_X2APIC;
    return ((uint32_t)rdmsr(IA32_APIC_BASE_MSR) & flags) == flags;
}

// Set the physical address of the APIC
void lapic_set_base(uintptr_t apic) {
    uint64_t eax = (apic & 0xfffff000) | IA32_APIC_BASE_MSR_ENABLE;
    wrmsr(IA32_APIC_BASE_MSR, eax);
}

// Return the physical address of the APIC
uintptr_t lapic_get_base() {
    return ((uintptr_t) rdmsr(IA32_APIC_BASE_MSR) & 0xfffff000);
}

// Read to an apic register
uint32_t lapic_register_read(uint32_t reg) {
    return *(volatile uint32_t*)(os.apic.lapic_base + reg);
}

// Write to an apic register
void lapic_register_write(uint32_t reg, uint32_t value) {
    *(volatile uint32_t*)(os.apic.lapic_base + reg) = value;
    (void) lapic_register_read(APIC_REG_ID);
}

uint32_t lapic_get_id() {
    return lapic_register_read(APIC_REG_ID) >> 24;
}

// Send a init command to another APIC
void lapic_send_init(uint32_t apic) {
    lapic_register_write(APIC_REG_INTERRUPT_CMD_HIGH, apic << 24);

    lapic_register_write(APIC_REG_INTERRUPT_CMD_LOW,
        APIC_ICR_CMD_INIT |
        APIC_ICR_PHYSICAL |
        APIC_ICR_EDGE     |
        APIC_ICR_ASSERT);

    while ( lapic_register_read(APIC_REG_INTERRUPT_CMD_LOW) & APIC_ICR_SEND_PENDING );
}

// Send a init command to another APIC
void lapic_send_startup(uint32_t apic, uint32_t vec) {
    lapic_register_write(APIC_REG_INTERRUPT_CMD_HIGH, apic << 24);

    lapic_register_write(APIC_REG_INTERRUPT_CMD_LOW,
        vec                  |
        APIC_ICR_CMD_STARTUP |
        APIC_ICR_PHYSICAL    |
        APIC_ICR_EDGE        |
        APIC_ICR_ASSERT);

    while ( lapic_register_read(APIC_REG_INTERRUPT_CMD_LOW) & APIC_ICR_SEND_PENDING );
}

// Send the End of Interrupt to the APIC
void lapic_eoi(uint8_t irq) {
    (void) irq;
    lapic_register_write(APIC_REG_END_OF_INTERRUPT, 0);
}

// Determine if the APIC is enabled
bool lapic_is_enabled() {
    uint32_t value = (uint32_t) rdmsr(IA32_APIC_BASE_MSR);
    return (value & IA32_APIC_BASE_MSR_ENABLE) > 0 && os.apic.lapic_base != 0;
}

// Enable the spurious interrupt vector
void lapic_enable_spurious_interrupt(uint32_t intr) {
    lapic_register_write(APIC_REG_SPURIOUS_INTERRUPT, intr | 0x100);
}

// Get the current in service routine for the current lapic
int lapic_inservice_routine() {
    for ( int i = 0; i < 8; i++ ) {
        uint32_t isr = lapic_register_read(APIC_REG_INSERVICE_ROUTINE + (0x10 * i));

        if ( isr > 0 ) {
            return ((i * 32) + __builtin_ctz(isr)) - 0x20;
        }
    }

    return -1;
}

// Clear error status register
void lapic_clear_error() {
    printf("lapic: error status: %x\n", lapic_register_read(APIC_REG_ERROR_STATUS));
    lapic_register_write(APIC_REG_ERROR_STATUS, 0);
    lapic_register_write(APIC_REG_ERROR_STATUS, 0);
}

INTERRUPT_HANDLER(spurious_dummy, {
    (void) regs;
    lapic_eoi(0);
})

// Enable the APIc
void lapic_enable(void) {
    // Hardware enable APIC
    uintptr_t base = lapic_get_base();
    // lapic_set_base(base);
    
    os.apic.lapic_base = (uintptr_t) MAP_SIZE(base, 4096, Present | ReadWrite);

    // printf("lapic: x2apic status %d\n", lapic_is_x2apic());

    lapic_register_write(APIC_REG_TASK_PRIORITY, 0);                // Enable all interrupts
    lapic_register_write(APIC_REG_DESTINATION_FORMAT, 0xFFFFFFFF);  // Flat mode
    lapic_register_write(APIC_REG_LOGICAL_DESTINATION, 1 << 24); // Logical CPU 1

    // Create spurious interrupt in IDT and enable it

    interrupt_set_gate(0xFF, (uintptr_t) spurious_dummy, 0x08, 0x8E);
    lapic_enable_spurious_interrupt(0xFF);


    // printf("lapic: setup at %p.\n", os.apic.lapic_base);
    // printf("lapic: id %d\n", (lapic_register_read(APIC_REG_ID) >> 24));
    // printf("lapic: version is %x\n", lapic_register_read(APIC_REG_VERSION));
}
