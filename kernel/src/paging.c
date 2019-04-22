#include "paging.h"
#include "console.h"
#include "memory.h"
#include "pheap.h"
#include "mheap.h"

#include "bound.h"

extern uint32_t page_table[1024][1024];
extern uint32_t page_directory[1024];

size_t page_bitmap[32768];
size_t superpage_bitmap[32];

multiboot_memory_map_t free_map[10] __attribute__((aligned(16)));
multiboot_memory_map_t res_map[10] __attribute__((aligned(16)));

static size_t free_counter = 0;
static size_t res_counter = 0;

size_t free_pages = 0;

extern void flush_tlb();

extern size_t _kernel_phys_start, _kernel_phys_end, _kernel_virt_start, _kernel_virt_end;

multiboot_memory_map_t* memory_usable(uintptr_t addr)
{
	for(size_t i = 0; i < free_counter; ++i)
	{
		if(bound_s(free_map[i].addr, free_map[i].addr + free_map[i].len, addr))
		{
			return &free_map[i];
		}
	}
	
	return 0;
}

void paging_init(multiboot_uint32_t mmap_addr, multiboot_uint32_t mmap_len)
{

	multiboot_memory_map_t* mmap = (void*) mmap_addr;
    //size_t kernel_size = (size_t) &_kernel_phys_end - (size_t) &_kernel_phys_start;

	memset((void*)page_bitmap, 0xFF, 32768*sizeof(size_t));
	memset((void*)superpage_bitmap, 0, sizeof(size_t) * 32);

	size_t free_memory_count = 0; // in bytes

	size_t virtual_addr = 0x01000000;
	
	while((size_t)mmap < (mmap_addr + mmap_len))
	{
		if(mmap->type != MULTIBOOT_MEMORY_AVAILABLE)
		{
			res_map[res_counter++] = *mmap;
			goto end;
		}

		#ifdef _KERNEL_CPU_32
		if((mmap->addr + mmap->len) > 0xFFFFFFFF)
		{
			goto end;
		}
		#endif

		if(mmap->len < 0x1000)
		{
			goto end;
		}

		//free_map[free_counter++] = *mmap;

		memcpy(&free_map[free_counter], mmap, sizeof(multiboot_memory_map_t));

		volatile size_t physical_address = mmap->addr; /* without volatile this whole loop gets skipped */
		
		free_memory_count += mmap->len;

		while(physical_address < (mmap->addr+mmap->len))
		{
			if(bound(virtual_addr, virtual_addr + 0x1000, (uintptr_t) &_kernel_virt_start, (uintptr_t) &_kernel_virt_end))
			{
				virtual_addr += 0x1000;
				continue;
			}

			if(bound(physical_address, physical_address + 0x1000, (uintptr_t) &_kernel_phys_start, (uintptr_t) &_kernel_phys_end))
			{
				physical_address += 0x1000;
				continue;
			}

			map_page(physical_address, virtual_addr, Present | ReadWrite 
			#ifndef _KERNEL_CPU_32
				| PageSize
			#endif
			);

			const size_t current_page = virtual_addr/0x1000;

			if(page_bitmap[current_page/32] == 0xFFFFFFFF)
			{
				--superpage_bitmap[current_page/0x1000];
			}

			page_bitmap[current_page / 32] &= ~(1 << (current_page%32));


			physical_address += 0x1000;
			virtual_addr += 0x1000;

			if(virtual_addr < 0x1000000)
			{
				printf("Memory truncated\n");
				break;
			}
		}
		
		end:
		mmap = (multiboot_memory_map_t*) ((size_t)mmap + mmap->size + sizeof(mmap->size));
	}
}

void map_page(uintptr_t physical, uintptr_t virtual, uint32_t flags)
{
	page_directory[virtual >> 22] |= 3;
    page_table[virtual >> 22][(virtual >> 12) & 0x3ff] = (physical & ~0xFFF) | flags;
	flush_tlb();
}

inline uintptr_t get_physaddr(uintptr_t virtual)
{
    return (page_table[virtual >> 22][(virtual >> 12) & 0x3ff] & ~0xFFF) + (virtual & 0xfff);
}
