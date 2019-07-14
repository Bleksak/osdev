#include "acpi.h"
#include "rsdp.h"
#include "ebda.h"
#include "paging.h"

#include <stddef.h>
#include <stdint.h>

#define RSD_PTR_ 0x2052545020445352

static union RSDP* RsdpFind(uint8_t* start, uint8_t* end)
{
    while(end > start)
    {
        if(*(uint64_t*)start == RSD_PTR_)
        {
            union RSDP* tmpRSDP = (union RSDP*) start;
            size_t len = (tmpRSDP->v1.Revision == 0) ? sizeof(struct _RsdpV1) : tmpRSDP->v2.Length;
            
            if(doChecksum(start, len))
            {
                return tmpRSDP;
            }
        }
        start += 16;
    }

    return 0;
}

union RSDP* RsdpInit(void)
{
    /*
        Look in the first kB in EBDA OR
        0x000E0000 to 0x000FFFFF
        
        length = 0x1FFFF
    */

    uint8_t* start = (uint8_t*)getEBDA();

    union RSDP* rsdp;

    if(!(rsdp = RsdpFind(start, start + 1024)))
    {
        
        uintptr_t current_addr = getCurrentVirtualMemoryOffset();
        const uintptr_t start = current_addr;

        for(uintptr_t phys_map_addr = 0xE0000; phys_map_addr < 0xFFFFF; phys_map_addr += 0x1000, current_addr += 0x1000)
        {
            map_page(phys_map_addr, current_addr, Present);
        }

        if(!(rsdp = RsdpFind((uint8_t*)start, (uint8_t*)(start + 0x1FFFF))))
        {
            return 0;
        }
    }

    return rsdp;
}