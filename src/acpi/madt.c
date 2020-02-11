#include "madt.h"
#include "acpi.h"
#include <interrupt/apic.h>
#include <interrupt/ioapic.h>

#include <paging.h>
#include <console.h>
#include <memory.h>
#include <mheap.h>
#include <os.h>

#include <pheap.h>
#include <align.h>

void madt_parse(const SDT* header) {
    const MADT* madt = (void*) header;
    const MADTEntry* current_entry = &madt->entries;

    os.apic.flags = madt->flags;

    void* tmp = 0;

    while((uintptr_t)current_entry < ((uintptr_t)madt + madt->sdt.length)) {
        
        switch(current_entry->type) {
            case MADT_LAPIC: {
                if(!aligncmp(os.apic.lapic.size * sizeof(LAPICEntry), (os.apic.lapic.size + 1) * sizeof(LAPICEntry), 0x1000) || !os.apic.lapic.entries) {
                    tmp = repalloc(os.apic.lapic.entries, os.apic.lapic.size, os.apic.lapic.size + 1);

                    os.apic.lapic.size++;

                    if(tmp)
                        os.apic.lapic.entries = tmp;

                }

                os.apic.lapic.entries[os.apic.lapic.length] = current_entry->lapic;
                os.apic.lapic.length++;
            } break;

            case MADT_IOAPIC: {
                if(!aligncmp(os.apic.ioapic.size * sizeof(IOAPICEntry), (os.apic.ioapic.size + 1) * sizeof(IOAPICEntry), 0x1000) || !os.apic.ioapic.entries) {
                    tmp = repalloc(os.apic.ioapic.entries, os.apic.ioapic.size, os.apic.ioapic.size + 1);

                    os.apic.ioapic.size++;
                    if(tmp)
                        os.apic.ioapic.entries = tmp;
                }

                os.apic.ioapic.entries[os.apic.ioapic.length] = current_entry->ioapic;
                os.apic.ioapic.length++;
            } break;

            case MADT_INTERRUPT_OVERRIDE: {

                if(!aligncmp(os.apic.interrupt_override.size * sizeof(InterruptOverrideEntry), (os.apic.interrupt_override.size + 1) * sizeof(InterruptOverrideEntry), 0x1000) || !os.apic.interrupt_override.entries) {
                    tmp = repalloc(os.apic.interrupt_override.entries, os.apic.interrupt_override.size, os.apic.interrupt_override.size + 1);

                    os.apic.interrupt_override.size++;
                    if(tmp)
                        os.apic.interrupt_override.entries = tmp;
                }

                os.apic.interrupt_override.entries[os.apic.interrupt_override.length] = current_entry->interrupt_override;
                os.apic.interrupt_override.length++;
            } break;

            case MADT_NMI: {
                if(!aligncmp(os.apic.nmi.size * sizeof(NMIEntry), (os.apic.nmi.size + 1) * sizeof(NMIEntry), 0x1000) || !os.apic.nmi.entries) {
                    tmp = repalloc(os.apic.nmi.entries, os.apic.nmi.size, os.apic.nmi.size + 1);

                    os.apic.nmi.size++;
                    if(tmp)
                        os.apic.nmi.entries = tmp;
                }

                os.apic.nmi.entries[os.apic.nmi.length] = current_entry->nmi;
                os.apic.nmi.length++;
            } break;
        }

        if(!tmp) {
            panic("Realloc failed (MADT)\n");
        }

        current_entry = (MADTEntry*)((uintptr_t)current_entry + current_entry->length);
    }

    

}
