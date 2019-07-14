#include "madt.h"
#include "apic.h"
#include "acpi.h"

#include "../paging.h"

bool initMADT(struct MADT_SDT* header)
{
    if(!header)
        return false;
    
    if(!doChecksum((uint8_t*)header, header->SDT.Length))
        return false;

    if((((uintptr_t)header + header->SDT.Length) & ~0xFFF) != ((uintptr_t)header & ~0xFFF))
    {
        const uintptr_t current_offset = getCurrentVirtualMemoryOffset();
        map_page(get_physaddr((uintptr_t)header) + 0x1000, current_offset, Present);
        setCurrentVirtualMemoryOffset(current_offset + 0x1000);
    }

    initAPIC(header);

    return true;
}