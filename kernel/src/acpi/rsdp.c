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

union RSDP* RsdpInit()
{
    /*
        Look in the first kB in EBDA OR
        0x000E0000 to 0x000FFFFF
    */

    uint8_t* start = (uint8_t*)getEBDA();

    union RSDP* rsdp;

    if(!(rsdp = RsdpFind(start, start + 1024)))
    {
        if(!(rsdp = RsdpFind((uint8_t*)(MEM_ZERO_MAP + 0xE0000), (uint8_t*)(MEM_ZERO_MAP + 0xFFFFF))))
        {
            return 0;
        }
    }

    return rsdp;
}