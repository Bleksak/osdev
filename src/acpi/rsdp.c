#include "acpi.h"
#include "rsdp.h"
#include "../ebda.h"
#include "../paging.h"

#define RSD_PTR_ 0x2052545020445352

static union RSDP* rsdp_find(uint8_t* start, uint8_t* end) {
    while(end > start) {
        if(*(uint64_t*)start == RSD_PTR_) {
            union RSDP* rsdp_tmp = (union RSDP*) start;
            size_t len = (rsdp_tmp->v1.revision == 0) ? sizeof(struct _RsdpV1) : rsdp_tmp->v2.length;
            
            if(do_checksum(start, len)) {
                return rsdp_tmp;
            }
        }
        start += 16;
    }

    return 0;
}

union RSDP* rsdp_init(void) {
    /*
        Look in the first kB in EBDA OR
        0x000E0000 to 0x000FFFFF
        
        length = 0x1FFFF
    */

    uint8_t* start = (uint8_t*)ebda_get();

    union RSDP* rsdp = rsdp_find(start, start + 1024);

    if(!rsdp) {
        uintptr_t current_addr = mem_offset_get();
        const uintptr_t start = current_addr;

        for(uintptr_t phys_map_addr = 0xE0000; phys_map_addr < 0xFFFFF; phys_map_addr += 0x1000, current_addr += 0x1000) {
            map_page(phys_map_addr, current_addr, Present);
        }
        
        rsdp = rsdp_find((uint8_t*)start, (uint8_t*)(start + 0x1FFFF));

        if(!rsdp) {
            return 0;
        }
    }

    return rsdp;
}