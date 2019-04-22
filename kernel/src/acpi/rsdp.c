#include "rsdp.h"
#include "ebda.h"
#include <stddef.h>

struct _RsdpV1
{
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
} __attribute__((packed));

struct _RsdpV2
{
    struct _RsdpV1 v1;
    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];
} __attribute__((packed));

union RSDP
{
    struct _RsdpV1 v1;
    struct _RsdpV2 v2;
} __attribute__((packed));

#include "console.h"

static bool doChecksum(uint8_t* ptr, size_t len)
{
    unsigned char checksum = 0;

    for(size_t i = 0; i < len; ++i)
    {
        checksum += ptr[i];
    }

    return (checksum == 0xFF);
}

static union RSDP* RsdpFind(uint8_t* start, uint8_t* end)
{
    while(end > start)
    {
        if(*(uint64_t*)start == 0x3282848032688382)
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

bool RsdpInit()
{
    /*
        Look in the first kB in EBDA OR
        0x000E0000 to 0x000FFFFF
    */

   /*
        RSD PTR =>  0x8283683280848232
         RTP DSR => 0x3282848032688382
   */

    uint8_t* start = (uint8_t*)getEBDA();
    printf("EBDA is at: %p\n", start);
    for(;;);
    union RSDP* rsdp;

    if(!(rsdp = RsdpFind(start, start + 1024)))
    {
        printf("Got to stage2\n");
        if(!(rsdp = RsdpFind(0, (uint8_t*)0xFFFFF)))
        {
            return false;
        }
    }

    printf("Found at %p\n", rsdp);

    return true;
}