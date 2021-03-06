#include "paging.h"
#include "console.h"
#include "memory.h"
#include "pheap.h"
#include "mheap.h"
#include "bound.h"
#include "assert.h"
#include "align.h"

/*
	BDA will be mapped at VIRTUAL_MEMORY_START
	EBDA will follow at VIRTUAL_MEMORY_START + 0x1000
*/


uint32_t page_table[1024][1024] __attribute__((aligned(4096))) = {{0}};
uint32_t page_directory[1024] __attribute__((aligned(4096))) = {0};


extern void page_unmark(size_t page);
extern void superpage_unmark(size_t page);

extern size_t page_bitmap[32768];
extern size_t superpage_bitmap[32];

enum MMAP_TYPE {
	MMAP_UNDEFINED,
	MMAP_FREE,
	MMAP_RESERVED,
	MMAP_ACPI_RECL,
	MMAP_ACPI_NVS,
	MMAP_COUNT,
};

static void flush_tlb(void) {
	const uintptr_t address = GET_PHYS(page_directory);

	__asm__ volatile(
		"mov %%eax, %%cr3" :: "a" (address)
	);
}

extern uintptr_t _kernel_phys_start, _kernel_phys_end, _kernel_virt_start, _kernel_virt_end;

static uintptr_t virtual_memory_offset = 0;

#define MAKE_PAIR(a, b) {(uintptr_t) a, (uintptr_t) b}
typedef struct { uintptr_t start, end; } BoundPair;

static const BoundPair physical_bound_checks[] = {
	MAKE_PAIR(0x0, 0xFFFFF), // don't map the first MB
	MAKE_PAIR(&_kernel_phys_start, &_kernel_phys_end),
};

static const BoundPair virtual_bound_checks[] = {
	MAKE_PAIR(0x0, 0xFFFFF), // don't map the first MB
	MAKE_PAIR(&_kernel_virt_start, &_kernel_virt_end)
};

static bool paging_check_physical_bounds(uintptr_t address) {
	for(uintptr_t i = 0; i < (sizeof(physical_bound_checks) / sizeof(physical_bound_checks[0])); ++i) {
		if(bound(physical_bound_checks[i].start, physical_bound_checks[i].end, address, address + 0xFFF)) {
			return true;
		}
	}

	return false;
}

static bool paging_check_virtual_bounds(uintptr_t address) {
	for(size_t i = 0; i < (sizeof(virtual_bound_checks) / sizeof(virtual_bound_checks[0])); ++i) {
		if(bound(virtual_bound_checks[i].start, virtual_bound_checks[i].end, address, address + 0xFFF)) {
			return true;
		}
	}

	return false;
}

static void map_page_local(uintptr_t physical, uintptr_t virtual) {
	page_directory[virtual >> 22] |= 3;
    page_table[virtual >> 22][(virtual >> 12) & 0x3ff] = (physical & ~0xFFF) | Present;
}

void paging_init(multiboot_uint32_t mmap_addr, multiboot_uint32_t mmap_len) {
	memset(page_bitmap, 0xFF, sizeof(page_bitmap));
	memset(superpage_bitmap, 0xFF, sizeof(superpage_bitmap));

	virtual_memory_offset = ((uintptr_t)&_kernel_virt_end + 0x1000) & ~(0xFFF); // use higher half for kernel purposes

	uintptr_t virtual_address = 0;

	for(multiboot_memory_map_t* mmap = (void*) mmap_addr;(uintptr_t)mmap < (mmap_addr + mmap_len); mmap = (multiboot_memory_map_t*) ((uintptr_t)mmap + mmap->size + sizeof(mmap->size))) {
		if(mmap->type == MULTIBOOT_MEMORY_BADRAM) {
			continue;
		}

		else if(mmap->type == MMAP_FREE) {
			const uint64_t limit = (uint64_t)mmap->addr + (uint64_t)mmap->len;
			
			for(uintptr_t physical_address = mmap->addr; physical_address < limit;) {
				if(limit - physical_address < 0x1000) {
					physical_address += 0x1000;
					continue;
				}
				
				// overflow check for 32 bit machines (64 bit shouldn't be able to overflow)
				#ifdef _x86
					if((limit > 0xFFFFFFFFULL) || (((uint64_t)virtual_address + 0x1000ULL) > 0xFFFFFFFFULL)) {
						printf("Memory map truncated (32 bit OS cannot access more than 4G)\n");
						return;
					}
				#endif
				
				if(paging_check_virtual_bounds(virtual_address)) {
					virtual_address += 0x1000;
					continue;
				}

				if(paging_check_physical_bounds(physical_address)) {
					physical_address += 0x1000;
					continue;
				}

				map_page_local(physical_address, virtual_address);

				const uintptr_t page = virtual_address >> 12;
				
				page_bitmap[page >> 5] &= ~(1 << (page & 0x1F));
				superpage_unmark(page);

				physical_address += 0x1000;
				virtual_address += 0x1000;
			}
		}
	}

	for(uintptr_t addr = 0; addr <= 0xA0000; addr+=0x1000) {
		unmap_page(addr);
	}

	flush_tlb();
	heap_init();
}

ptrdiff_t map_page(uintptr_t physical, uintptr_t virtual, uint32_t flags) {
	page_directory[virtual >> 22] |= 3;
    page_table[virtual >> 22][(virtual >> 12) & 0x3ff] = (physical & ~0xFFF) | flags;
	flush_tlb();

	return (ptrdiff_t)physical & 0xFFF;
}

void unmap_page(uintptr_t address) {
	page_table[address >> 22][(address >> 12) & 0x3ff] = 0;
	flush_tlb();
}

const void* map_size(uintptr_t physical, size_t size, uint32_t flags) {
	const size_t aligned_size = align(size, 0x1000);

	const size_t phys_addr = physical & ~0xFFF;
	const uintptr_t end = phys_addr + aligned_size;

	const void* return_address = (const void*) virtual_memory_offset;

	for(uintptr_t addr = phys_addr; addr < end; addr += 0x1000, virtual_memory_offset += 0x1000) {
		map_page(addr, virtual_memory_offset, flags);
	}

	return return_address;
}

inline uintptr_t get_physical_address(uintptr_t virtual) {
    return (page_table[virtual >> 22][(virtual >> 12) & 0x3ff] & ~0xFFF) | (virtual & 0xfff);
}

inline void page_set_flags(size_t page, uint32_t flags) {
	page_table[page >> 10][page & 0x3ff] |= flags;
}

inline void page_unset_flags(size_t page, uint32_t flags) {
	page_table[page >> 10][page & 0x3ff] &= ~flags;
}

uintptr_t mem_offset_get(void) {
	return virtual_memory_offset;
}

void mem_offset_set(uintptr_t address) {
	virtual_memory_offset = address;
}
