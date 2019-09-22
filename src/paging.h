#pragma once
#include "std.h"

#include "multiboot.h"

#include "attributes.h"

#define VIRTUAL_MEMORY_START 0x01000000
#define PUBLIC_VIRTUAL_MEMORY_START VIRTUAL_MEMORY_START + 0x4000000 // 64MiB reserved for OS
#define MEM_ZERO_MAP VIRTUAL_MEMORY_START

#define KB 1024
#define MB KB * 1024
#define GB MB * 1024

enum PageFlags
{
    Present = 1 << 0,
    ReadWrite = 1 << 1,
    PrivillegeLevel = 1 << 2,
    WriteThrough = 1 << 3,
    CacheDisable = 1 << 4,
    Accessed = 1 << 5,
    Dirty = 1 << 6,
    PageSize = 1 << 7, // 0 4KiB, 1 4MiB
    Global = 1 << 8,
};

#define GET_PHYS(addr) get_physical_address(((uintptr_t)addr))

PURE uintptr_t mem_offset_get(void);
void mem_offset_set(uintptr_t address);

void paging_init(multiboot_uint32_t mmap_addr, multiboot_uint32_t mmap_len);
ptrdiff_t map_page(uintptr_t physical, uintptr_t virtual, uint32_t flags);
uintptr_t get_physical_address(uintptr_t virtual);
uintptr_t readCR3(void);
