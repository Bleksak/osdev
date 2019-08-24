#include "acpi.h"
#include "rsdp.h"
#include "../paging.h"
#include "../panic.h"

// Include all other possible SDT pointers
#include "madt.h"

static uint32_t AcpiCapabilities = 0;

PURE uint32_t getAcpiCapabilities(void)
{
    return AcpiCapabilities;
}

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
    SDT_HeaderEnumSize,
};

struct AcpiSDTHeader
{
    struct SDT_Header header;
    uint32_t NextSDT[];
} PACKED;

#include "../console.h"

static const struct SDT_Header* findAcpiTable(struct AcpiSDTHeader* table, const size_t entryCount, const uint32_t name)
{
    for(size_t i = 0; i < entryCount; ++i)
    {

        // printf("%s\n", (char*)table->NextSDT[i]);

        if(*(uint32_t*)table->NextSDT[i] == name)
        {
            const struct SDT_Header* found = (const struct SDT_Header*) table->NextSDT[i];
            if(doChecksum((uint8_t*)found, found->Length))
            {
                return found;
            }
        }
    }


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

static void setAcpiCapabilities(enum ACPI_Capabilities cap)
{
    AcpiCapabilities |= cap;
}

bool AcpiInit(void)
{
    const uint32_t SDT_HeaderStringTable[] =
    {
        0x43495041,
        0x54524542,
        0x50455043,
        0x54445344,
        0x54444345,
        0x4a4e4945,
        0x54535245,
        0x50434146,
        0x53434146,
        0x54534548,
        0x5443534d,
        0x5453504d,
        0x004d454f,
        0x54544d50,
        0x54445350,
        0x46534152,
        0x54445352,
        0x53425354,
        0x54494c53,
        0x54415253,
        0x54445353,
        0x54445358,
    };

    union RSDP* rsdp;
    if(!(rsdp = RsdpInit()))
    {
        return false;
    }

    uintptr_t mem_offset = getCurrentVirtualMemoryOffset();
    const uintptr_t addr = (rsdp->v1.Revision == 0) ? rsdp->v1.RsdtAddress : rsdp->v2.XsdtAddress;

    ptrdiff_t acpi_offset = map_page(addr, mem_offset, Present | ReadWrite);

    setCurrentVirtualMemoryOffset(mem_offset + 0x1000);

    struct AcpiSDTHeader* AcpiSDT = (struct AcpiSDTHeader*)(mem_offset + acpi_offset);

    mem_offset += 0x1000;

    if(!doChecksum((uint8_t*)AcpiSDT, AcpiSDT->header.Length))
    {
        return false;
    }

    const size_t entryCount = (AcpiSDT->header.Length - sizeof(struct AcpiSDTHeader)) >> ((rsdp->v1.Revision == 0) ? 2 : 3);

    uintptr_t current_addr = AcpiSDT->NextSDT[0] & ~0xFFF;
    AcpiSDT->NextSDT[0] = mem_offset + map_page(AcpiSDT->NextSDT[0], mem_offset, Present);

    for(size_t i = 1; i < entryCount; ++i)
    {
        if((AcpiSDT->NextSDT[i] & ~0xFFF) == current_addr)
        {
            AcpiSDT->NextSDT[i] = mem_offset + (AcpiSDT->NextSDT[i] & 0xFFF);
        }
        else
        {
            AcpiSDT->NextSDT[i] = mem_offset + map_page(AcpiSDT->NextSDT[i], mem_offset, Present);

            mem_offset += 0x1000;
            current_addr = AcpiSDT->NextSDT[i] & ~0xFFF;
        }

        const struct SDT_Header* header = (const struct SDT_Header*) AcpiSDT->NextSDT[i];

        if(((current_addr + header->Length) & ~0xFFF) != current_addr)
        {
            map_page(current_addr + header->Length, mem_offset, Present);
            mem_offset += 0x1000;
            current_addr = (current_addr + header->Length) & ~0xFFF;
        }
    }

    setCurrentVirtualMemoryOffset(mem_offset + 0x1000);

    if(initMADT((struct MADT_SDT*)findAcpiTable(AcpiSDT, entryCount, SDT_HeaderStringTable[MADT])))
    {
        setAcpiCapabilities(ACPI_MADT);
    }

    return true;
}
