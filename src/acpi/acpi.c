#include "acpi.h"
#include "rsdp.h"
#include "../paging.h"

#include "madt.h"

#include "../bound.h"

#include "../memory.h"
#include "../mheap.h"
#include "../os.h"


enum SDT_HeaderEnum {
    SDT_MADT,
    SDT_BERT,
    SDT_CPEP,
    SDT_DSDT,
    SDT_ECDT,
    SDT_EINJ,
    SDT_ERST,
    SDT_FADT,
    SDT_FACS,
    SDT_HEST,
    SDT_MSCT,
    SDT_MPST,
    SDT_OEMx,
    SDT_PMTT,
    SDT_PSDT,
    SDT_RASF,
    SDT_RSDT,
    SDT_SBST,
    SDT_SLIT,
    SDT_SRAT,
    SDT_SSDT,
    SDT_XSDT,
    SDT_COUNT,
};

static const uint32_t sdt_string_table[] = {
    0x43495041, //madt
    0x54524542, //bert
    0x50455043, //cpep
    0x54445344, //dsdt
    0x54444345, //ecdt
    0x4a4e4945, //einj
    0x54535245, //erst
    0x50434146, //fadt
    0x53434146, //facs
    0x54534548, //hest
    0x5443534d, //msct
    0x5453504d, //mpst
    0x004d454f, //oemx
    0x54544d50, //pmtt
    0x54445350, //psdt
    0x46534152, //rasf
    0x54445352, //rsdt
    0x53425354, //rasf
    0x54494c53, //rsdt
    0x54415253, //sbst
    0x54445353, //slit
    0x54445358, //srat
};

typedef void (*table_parser)(const SDT* sdt);

static table_parser parsers[] = {
    madt_parse,
};

struct AcpiSDT {
    SDT header;
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

// static const void* acpi_find_table(struct AcpiSDT* table, const size_t entry_count, const uint32_t name) {
//     for(size_t i = 0; i < entry_count; ++i) {
//         if(*(uint32_t*)table->next_sdt[i] == name) {
//             const struct SDT* found = (const struct SDT*) table->next_sdt[i];
//             if(do_checksum((const uint8_t*)found, found->length)) {
//                 return found;
//             }
//         }
//     }

//     return 0;
// }


// static void acpi_set_capabilities(enum ACPI_Capabilities cap) {
//     acpi_capabilities |= cap;
// }

static bool parse_table(const SDT* sdt) {
    const uint32_t signature = *(const uint32_t*) sdt->signature;

    for(size_t i = 0; i < SDT_COUNT; ++i) {
        if((signature & sdt_string_table[i]) == sdt_string_table[i]) {
            if(do_checksum((const uint8_t*) sdt, sdt->length)) {
                if(parsers[i]) {
                    parsers[i](sdt);
                    return true;
                }
            }
        }
    }

    printf("Warning: %c%c%c%c does not have a parser.\n", sdt->signature[0], sdt->signature[1], sdt->signature[2], sdt->signature[3]);
    return true; // TODO: change to false later
}

bool acpi_init(void) {
    union RSDP* rsdp = rsdp_init();

    if(!rsdp) {
        return false;
    }

    const uintptr_t addr = (rsdp->v1.revision == 0) ? rsdp->v1.rsdt_address : rsdp->v2.xsdt_address;
    
    struct AcpiSDT* acpi_sdt = MAP_SIZE(addr, sizeof(struct AcpiSDT), Present | ReadWrite);

    if(!do_checksum((uint8_t*)acpi_sdt, acpi_sdt->header.length)) {
        return false;
    }

    const size_t entry_count = (acpi_sdt->header.length - sizeof(struct AcpiSDT)) >> ((rsdp->v1.revision == 0) ? 2 : 3);

    uintptr_t last_offset = 0;

    void* acpi_tables = 0;
    uintptr_t* pointers = malloc(entry_count * sizeof(uintptr_t));

    if(!pointers) {
        panic("Couldn't parse ACPI tables, allocator crashed");
    }

    for(size_t i = 1; i < entry_count; ++i) {
        const uintptr_t current_ptr = acpi_sdt->next_sdt[i];
        const SDT* current_header = (const void*) map_page(current_ptr, 0, Present);
        
        const ptrdiff_t old_offset = last_offset;

        last_offset += current_header->length;
        
        acpi_tables = realloc(acpi_tables, last_offset);

        if(!acpi_tables) {
            panic("Couldn't parse ACPI tables, allocator crashed");
        }

        pointers[i] = (uintptr_t)acpi_tables + old_offset;
        memcpy( (void*) ((uintptr_t) acpi_tables + old_offset), (void* restrict) current_header, current_header->length);

        if(!parse_table( (const SDT*) ((uintptr_t)acpi_tables + old_offset))) {
            panic("Failed to parse table %s", (const SDT*) ((uintptr_t)acpi_tables + old_offset));
        }
    }

    return true;
}
