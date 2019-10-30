#include "madt.h"
#include "acpi.h"
#include "../smp/apic.h"
#include "../smp/ioapic.h"

#include "../paging.h"
#include "../console.h"
#include "../memory.h"
#include "../mheap.h"
#include "../os.h"

void madt_parse(const SDT* header) {
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

        if(!tmp) {
            panic("Realloc failed (MADT)\n");
        }

        current_entry = (MADTEntry*)((uintptr_t)current_entry + current_entry->length);
    }
}
