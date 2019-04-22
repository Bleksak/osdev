#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "multiboot.h"

enum PageFlags
{
    Present = 1 << 0,
    ReadWrite = 1 << 1,
    PrivillegeLevel = 1 << 2,
    WriteThrough = 1 << 3,
    CacheDisable = 1 << 4,
    Accessed = 1 << 5,
    Dirty = 1 << 6,
    PageSize = 1 << 7,
    Global = 1 << 8,
};

void paging_init(multiboot_uint32_t mmap_addr, multiboot_uint32_t mmap_len);
void map_page(uintptr_t physical, uintptr_t virtual, uint32_t flags);
uintptr_t get_physaddr(uintptr_t virtual);
