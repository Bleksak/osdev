#include "madt.h"
#include "acpi.h"
#include "../smp/apic.h"
#include "../smp/ioapic.h"

#include "../paging.h"
#include "../console.h"
#include "../memory.h"
#include "../mheap.h"
#include "../os.h"

bool madt_parse(const struct SDT* header) {
    const MADT* madt = (void*) header;
    const MADTEntry* current_entry = &madt->entries;

    os.apic.flags = madt->flags;

    while((uintptr_t)current_entry < ((uintptr_t)madt + madt->sdt.length)) {
        void* tmp;
        
        switch(current_entry->type) {
            case MADT_LAPIC: {
                VectorPush(os.apic.lapic, current_entry->lapic, tmp);
            } break;

            case MADT_IOAPIC: {
               VectorPush(os.apic.ioapic, current_entry->ioapic, tmp);
            } break;

            case MADT_INTERRUPT_OVERRIDE: {
                VectorPush(os.apic.interrupt_override, current_entry->interrupt_override, tmp);
            } break;

            case MADT_NMI: {
                VectorPush(os.apic.nmi, current_entry->nmi, tmp);
            } break;
        }
        
        // switch(current_entry->type) {
        //     case MADT_LAPIC: {
        //         os.apic.cpu_apics = realloc(os.apic.cpu_apics, (os.apic.cpu_count + 1) * sizeof(struct ProcessorLocalAPIC));
        //         memcpy(&os.apic.cpu_apics[os.apic.cpu_count], (void*) &current_entry->lapic, sizeof(struct ProcessorLocalAPIC));
        //         os.apic.cpu_count++;
        //     } break;

        //     case MADT_IOAPIC: {
        //         os.apic.io_apics = realloc(os.apic.io_apics, (os.apic.ioapic_count + 1) * sizeof(struct IOAPIC));
        //         memcpy(&os.apic.io_apics[os.apic.ioapic_count], (void*) &current_entry->io_apic, sizeof(struct IOAPIC));
        //         os.apic.io_apics[os.apic.ioapic_count].IOAPICAddress = (uintptr_t) MAP_SIZE(current_entry->io_apic.IOAPICAddress, sizeof(uintptr_t) * 64, Present | ReadWrite);

        //         os.apic.ioapic_count++;
        //     } break;

        //     case MADT_INTERRUPT_OVERRIDE: {
        //         os.apic.interrupt_source_override = realloc(os.apic.interrupt_source_override, (os.apic.override_count + 1) * sizeof(struct InterruptSourceOverride));
        //         memcpy(&os.apic.interrupt_source_override[os.apic.override_count], (void*) &current_entry->interrupt_source_override, sizeof(struct InterruptSourceOverride));
        //         os.apic.override_count++;
        //     } break;

        //     case MADT_NMI: {
        //         os.apic.non_maskable_interrupts = realloc(os.apic.non_maskable_interrupts, (os.apic.nmi_count + 1) * sizeof(struct NonMaskableInterrupt));
        //         memcpy(&os.apic.non_maskable_interrupts[os.apic.nmi_count], (void*) &current_entry->non_maskable_interrupt, sizeof(struct NonMaskableInterrupt));
        //         os.apic.nmi_count++;
        //     } break;

        //     case MADT_LAPIC_OVERRIDE: {
        //         lapic_addr = current_entry->lapic_addr_override.address;
        //     } break;
        // }

        if(!tmp) {
            panic("Realloc failed (MADT)\n");
        }

        current_entry = (MADTEntry*)((uintptr_t)current_entry + current_entry->length);
    }

    // ioapic_setup(os.apic.io_apics[0].IOAPICAddress);
    
    return true;
}
