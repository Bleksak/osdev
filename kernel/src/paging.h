#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "multiboot.h"
#include "size_t.h"

#define identity_map(addr) map_page((addr), (addr))


multiboot_memory_map_t kernel_map_entry[15];

struct kernel_map
{
    uint32_t count;
    multiboot_memory_map_t* map;
} kmap;

void paging_init(multiboot_uint32_t mmap_addr, multiboot_uint32_t mmap_len);
void map_page(uintptr_t phys, uintptr_t virt);
uintptr_t get_physaddr(uintptr_t virtual);
