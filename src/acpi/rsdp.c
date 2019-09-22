#include "acpi.h"
#include "rsdp.h"
#include "../ebda.h"
#include "../paging.h"

#define RSD_PTR_ 0x2052545020445352

static union RSDP* rsdp_find(const uint8_t* start, const uint8_t* end) {
    while(end > start) {
        if(*(const uint64_t*)start == RSD_PTR_) {
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

    const uint8_t* start = (void*) ebda_get();

    union RSDP* rsdp = rsdp_find(start, start + 1024);

    if(!rsdp) {
        start = MAP_SIZE(0xE0000, 0x1FFFF, Present);
        return rsdp_find(start, start + 0x1FFFF);
    }

    return rsdp;
}
