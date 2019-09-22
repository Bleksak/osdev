#include "madt.h"
#include "apic.h"
#include "acpi.h"

#include "../paging.h"

bool madt_init(const struct MADT_SDT* header) {
    if(!header) {
        return false;
    }
    
    if(!do_checksum((uint8_t*)header, header->sdt.length)) {
        return false;
    }

    if((((uintptr_t)header + header->sdt.length) & ~0xFFF) != ((uintptr_t)header & ~0xFFF)) {
        const uintptr_t current_offset = mem_offset_get();
        map_page(get_physical_address((uintptr_t)header) + 0x1000, current_offset, Present);
        mem_offset_set(current_offset + 0x1000);
    }

    apic_init(header);

    return true;
}
