#include "os.h"
#include <console.h>
#include <bda.h>

#include <acpi/acpi.h>
#include <memory.h>

#include <interrupt/ioapic.h>

struct OS os = {0};

void os_init(void) {
    memset_classic(&os, 0, sizeof(struct OS));

	cpu_init(&os.cpuid);
    bda_init();
    acpi_init();
    lapic_enable();
    os.interrupt.irq_vector[KEYBOARD_IRQ] = 32 + KEYBOARD_IRQ;
    
    os.interrupt.dispatcher = interrupt_find_dispatcher_id(DISPATCHER_PIC);
    os.interrupt.dispatcher->install();

    os.interrupt.dispatcher = interrupt_find_dispatcher_id(DISPATCHER_IOAPIC);
    os.interrupt.dispatcher->install();


    __asm__ volatile("sti");
}
