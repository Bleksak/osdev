#include "acpi.h"
#include "rsdp.h"
#include "../paging.h"
#include "../panic.h"

// Include all other possible SDT pointers
#include "madt.h"

static uint32_t acpi_capabilities = 0;

uint32_t acpi_get_capabilities(void) {
    return acpi_capabilities;
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

struct AcpiSDTHeader {
    struct SDT_Header header;
    uintptr_t next_sdt[];
} PACKED;

#include "../console.h"

inline bool do_checksum(const uint8_t* ptr, size_t len) {
    uint8_t checksum = 0;

    for(size_t i = 0; i < len; ++i) {
        checksum += ptr[i];
    }

    return (checksum == 0);
}

static const void* acpi_find_table(struct AcpiSDTHeader* table, const size_t entry_count, const uint32_t name) {
    for(size_t i = 0; i < entry_count; ++i) {
        if(*(uint32_t*)table->next_sdt[i] == name) {
            const struct SDT_Header* found = (const struct SDT_Header*) table->next_sdt[i];
            if(do_checksum((const uint8_t*)found, found->length)) {
                return found;
            }
        }
    }

    return 0;
}

// static size_t loop_table() {

// }


static void acpi_set_capabilities(enum ACPI_Capabilities cap) {
    acpi_capabilities |= cap;
}

bool acpi_init(void) {
    const uint32_t SDT_HeaderStringTable[] = {
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

    union RSDP* rsdp = rsdp_init();

    if(!rsdp) {
        return false;
    }

    const uintptr_t addr = (rsdp->v1.revision == 0) ? rsdp->v1.rsdt_address : rsdp->v2.xsdt_address;
    
    struct AcpiSDTHeader* acpi_sdt = MAP_SIZE(addr, sizeof(struct AcpiSDTHeader), Present | ReadWrite);

    if(!do_checksum((uint8_t*)acpi_sdt, acpi_sdt->header.length)) {
        return false;
    }

    const size_t entry_count = (acpi_sdt->header.length - sizeof(struct AcpiSDTHeader)) >> ((rsdp->v1.revision == 0) ? 2 : 3);

    const uintptr_t mem_start = mem_offset_get();

    for(size_t i = 0; i < entry_count; ++i) {
        uintptr_t current_ptr = ...
    }

    uintptr_t current_addr = acpi_sdt->next_sdt[0] & ~0xFFF;
    acpi_sdt->next_sdt[0] = mem_offset + map_page(acpi_sdt->next_sdt[0], mem_offset, Present);

    for(size_t i = 1; i < entry_count; ++i) {
        if((acpi_sdt->next_sdt[i] & ~0xFFF) == current_addr) {
            acpi_sdt->next_sdt[i] = mem_offset + (acpi_sdt->next_sdt[i] & 0xFFF);
        }
        else {
            acpi_sdt->next_sdt[i] = mem_offset + map_page(acpi_sdt->next_sdt[i], mem_offset, Present);

            mem_offset += 0x1000;
            current_addr = acpi_sdt->next_sdt[i] & ~0xFFF;
        }

        const struct SDT_Header* header = (const struct SDT_Header*) acpi_sdt->next_sdt[i];

        if(((current_addr + header->length) & ~0xFFF) != current_addr) {
            map_page(current_addr + header->length, mem_offset, Present);
            mem_offset += 0x1000;
            current_addr = (current_addr + header->length) & ~0xFFF;
        }
    }

    mem_offset_set(mem_offset + 0x1000);

    const void* madt = acpi_find_table(acpi_sdt, entry_count, SDT_HeaderStringTable[MADT]);
    if(madt) {
        if(madt_init(madt)) {
            acpi_set_capabilities(ACPI_MADT);
        }
    }

    return true;
}
