#include "paging.h"
#include "console.h"
#include "memory.h"
#include "pheap.h"
#include "mheap.h"

#include "bound.h"

#include "assert.h"

/*
	BDA will be mapped at 0x01000000 or VIRTUAL_MEMORY_START
	EBDA will follow at VIRTUAL_MEMORY_START + 0x1000
*/


extern uint32_t page_table[1024][1024];
extern uint32_t page_directory[1024];

size_t page_bitmap[32768] = {0};
size_t superpage_bitmap[32] = {0};

enum MMAP_TYPE {
	MMAP_FREE,
	MMAP_RESERVED,
	MMAP_ACPI_RECL,
	MMAP_ACPI_NVS,
	MMAP_COUNT,
};

static multiboot_memory_map_t maps[4][8] = {
	{{0}}, 
	{{0}}, 
	{{0}}, 
	{{0}}
};

static size_t counters[4] = {
	0, 0, 0, 0
};

extern void flush_tlb(void);

extern size_t _kernel_phys_start, _kernel_phys_end, _kernel_virt_start, _kernel_virt_end;

static uintptr_t virtual_memory_offset = VIRTUAL_MEMORY_START;


// multiboot_memory_map_t* memory_usable(uintptr_t addr)
// {
// 	for(size_t i = 0; i < free_counter; ++i)
// 	{
// 		if(bound_s(free_map[i].addr, free_map[i].addr + free_map[i].len, addr))
// 		{
// 			return &free_map[i];
// 		}
// 	}
	
// 	return 0;
// }

#define MAKE_PAIR(a, b) {(uintptr_t)a, (uintptr_t)b}
typedef struct { uintptr_t start, end; } BoundPair;



static const BoundPair physical_bound_checks[] = {
	MAKE_PAIR(0x0, 0xFFFFF), // don't map the first MB
	MAKE_PAIR(&_kernel_phys_start, &_kernel_virt_end),
};

static const BoundPair virtual_bound_checks[] = {
	MAKE_PAIR(&_kernel_virt_start, &_kernel_virt_end)
};

void paging_init(multiboot_uint32_t mmap_addr, multiboot_uint32_t mmap_len)
{
	memset(page_bitmap, 0xFF, sizeof(page_bitmap));
	memset(superpage_bitmap, 0xFF, sizeof(superpage_bitmap));
	
    //size_t kernel_size = (size_t) &_kernel_phys_end - (size_t) &_kernel_phys_start;

	volatile uintptr_t virtual_address = PUBLIC_VIRTUAL_MEMORY_START;

	for(multiboot_memory_map_t* mmap = (void*) mmap_addr; (uintptr_t) mmap < (mmap_addr + mmap_len); 
		     mmap = (multiboot_memory_map_t*) ((uintptr_t)mmap + mmap->size + sizeof(mmap->size))) {
		
		if(mmap->type == MULTIBOOT_MEMORY_BADRAM) {
			continue;
		}

		const enum MMAP_TYPE type = mmap->type - 1;
		size_t* counter = &counters[type];
		multiboot_memory_map_t* map = &maps[type][*counter];
		
		memcpy(map, mmap, sizeof(multiboot_memory_map_t));

		assert(map->addr == mmap->addr);

		if(type == MMAP_FREE) {

			const size_t limit = mmap->addr + mmap->len;
			
			for(uintptr_t physical_address = mmap->addr; physical_address < limit;) {

				

				if(limit - physical_address < 0x1000) {
					continue;
				}

				// overflow check for 32 bit machines
				#ifdef _x86
					if((limit > 0xFFFFFFFF) || (((unsigned long long)virtual_address + 0x1000ULL) > 0xFFFFFFFFULL)) {
						printf("Memory map truncated (32 bit OS cannot access more than 4G)\n");
					}

					return;
				#endif

				// Phyiscal address cannot overlap the physical kernel, otherwise we would be able to overwrite it.
				// Virtual address cannot overlap the virtual kernel, otherwise we wouldn't be able to access it.
				// so we need to add bound checking

				for(size_t i = 0; i < (sizeof(physical_bound_checks) / sizeof(physical_bound_checks[0])); ++i) {
					if(bound(physical_bound_checks[i].start, physical_bound_checks[i].end, physical_address, physical_address + 0xFFF)) {
						physical_address += 0x1000;
						continue;
					}
				}

				for(size_t i = 0; i < (sizeof(virtual_bound_checks) / sizeof(virtual_bound_checks[0])); ++i) {
					if(bound(virtual_bound_checks[i].start, virtual_bound_checks[i].end, virtual_address, virtual_address + 0xFFF)) {
						virtual_address += 0x1000; // need to increment only virtual address
						continue;
					}
				}
				// printf("%x\n", limit);
				map_page(physical_address, virtual_address, Present | ReadWrite);

				physical_address += 0x1000;
				virtual_address += 0x1000;
			}
		}

		(*counter)++;
	}
}

ptrdiff_t map_page(uintptr_t physical, uintptr_t virtual, uint32_t flags)
{
	page_directory[virtual >> 22] |= 3;
    page_table[virtual >> 22][(virtual >> 12) & 0x3ff] = (physical & ~0xFFF) | flags;
	flush_tlb();

	return (ptrdiff_t)physical & 0xFFF;
}

inline uintptr_t get_physaddr(uintptr_t virtual)
{
    return (page_table[virtual >> 22][(virtual >> 12) & 0x3ff] & ~0xFFF) + (virtual & 0xfff);
}

uintptr_t getCurrentVirtualMemoryOffset(void)
{
	return virtual_memory_offset;
}

void setCurrentVirtualMemoryOffset(uintptr_t address)
{
	assert(address < PUBLIC_VIRTUAL_MEMORY_START);
	virtual_memory_offset = address;
}