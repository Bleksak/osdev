#include "acpi.h"
#include "rsdp.h"
#include "paging.h"

enum SDT_HeaderEnum
{
    MADT,
    BERT,
    CPEP,
    DSDT,
    ECDT,
    EINJ,
    ERST,
    FADT,
    FACS,
    HEST,
    MSCT,
    MPST,
    OEMx,
    PMTT,
    PSDT,
    RASF,
    RSDT,
    SBST,
    SLIT,
    SRAT,
    SSDT,
    XSDT,
};

static const char* SDT_HeaderStringTable[] = 
{
    "APIC",
    "BERT",
    "CPEP",
    "DSDT",
    "ECDT",
    "EINJ",
    "ERST",
    "FACP",
    "FACS",
    "HEST",
    "MSCT",
    "MPST",
    "OEM",
    "PMTT",
    "PSDT",
    "RASF",
    "RSDT",
    "SBST",
    "SLIT",
    "SRAT",
    "SSDT",
    "XSDT",
};

static struct SDT_Header* findTable(uint8_t* start, uint8_t* end, const char* name)
{
    (void) start;
    (void) end;
    (void) name;
    return 0;
}

inline bool doChecksum(uint8_t* ptr, size_t len)
{
    uint8_t checksum = 0;

    for(size_t i = 0; i < len; ++i)
    {
        checksum += ptr[i];
    }

    return (checksum == 0);
}

bool AcpiInit()
{
    union RSDP* rsdp;
    if(!(rsdp = RsdpInit()))
    {
        return false;
    }

    const uintptr_t mem_offset = getCurrentVirtualMemoryOffset();
    const uintptr_t addr = (rsdp->v1.Revision == 0) ? rsdp->v1.RsdtAddress : rsdp->v2.XsdtAddress;

    map_page(addr, mem_offset, Present);
    setCurrentVirtualMemoryOffset(mem_offset + 0x1000);

    return true;
}