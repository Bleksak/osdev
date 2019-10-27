#include "madt.h"
#include "apic.h"
#include "acpi.h"

#include "../paging.h"
#include "../console.h"
#include "../memory.h"
#include "../mheap.h"
#include "../os.h"
#include "ioapic.h"

bool madt_parse(const struct SDT* header) {
    const struct MADT_SDT* madt = (void*) header;
    const struct MADT_Entry* current_entry = &madt->entries;

    uintptr_t lapic_addr = madt->lapic_addr;
    os.apic.flags = madt->flags;

    while((uintptr_t)current_entry < ((uintptr_t)madt + madt->sdt.length)) {
        switch(current_entry->type) {
            case ProcessorLocalAPIC: {
                os.apic.cpu_apics = realloc(os.apic.cpu_apics, (os.apic.cpu_count + 1) * sizeof(struct ProcessorLocalAPIC));
                memcpy(&os.apic.cpu_apics[os.apic.cpu_count], (void*) &current_entry->lapic, sizeof(struct ProcessorLocalAPIC));
                os.apic.cpu_count++;
            } break;

            case IOAPIC: {
                os.apic.io_apics = realloc(os.apic.io_apics, (os.apic.ioapic_count + 1) * sizeof(struct IOAPIC));
                memcpy(&os.apic.io_apics[os.apic.ioapic_count], (void*) &current_entry->io_apic, sizeof(struct IOAPIC));
                os.apic.io_apics[os.apic.ioapic_count].IOAPICAddress = (uintptr_t) MAP_SIZE(current_entry->io_apic.IOAPICAddress, sizeof(uintptr_t) * 64, Present | ReadWrite);

                os.apic.ioapic_count++;
            } break;

            case InterruptSourceOverride: {
                os.apic.interrupt_source_override = realloc(os.apic.interrupt_source_override, (os.apic.interrupt_source_override_count + 1) * sizeof(struct InterruptSourceOverride));
                memcpy(&os.apic.interrupt_source_override[os.apic.interrupt_source_override_count], (void*) &current_entry->interrupt_source_override, sizeof(struct InterruptSourceOverride));
                os.apic.interrupt_source_override_count++;
            } break;

            case NonMaskableInterrupts: {
                os.apic.non_maskable_interrupts = realloc(os.apic.non_maskable_interrupts, (os.apic.non_maskable_interrupts_count + 1) * sizeof(struct NonMaskableInterrupt));
                memcpy(&os.apic.non_maskable_interrupts[os.apic.non_maskable_interrupts_count], (void*) &current_entry->non_maskable_interrupt, sizeof(struct NonMaskableInterrupt));
                os.apic.non_maskable_interrupts_count++;
            } break;

            case LocalAPICAddressOverride: {
                lapic_addr = current_entry->lapic_addr_override.address;
            } break;
        }

        current_entry = (struct MADT_Entry*)((uintptr_t)current_entry + current_entry->length);
    }

    os.apic.lapic_addr = (uintptr_t) MAP_SIZE(lapic_addr, madt->sdt.length, Present | ReadWrite);
    lapic_enable();
    ioapic_setup(os.apic.io_apics[0].IOAPICAddress);

    return true;
}
