#include "paging.h"
#include "console.h"
#include "memory.h"
#include "pheap.h"
#include "mheap.h"

extern uint32_t page_table[1024][1024];
extern uint32_t page_directory[1024];

size_t page_bitmap[32768];
size_t superpage_bitmap[32];

multiboot_memory_map_t* free_map[10];
multiboot_memory_map_t* res_map[10];

static size_t free_counter = 0;
static size_t res_counter = 0;

size_t free_pages = 0;

extern void flush_tlb();

extern size_t _kernel_phys_start, _kernel_phys_end, _kernel_virt_start, _kernel_virt_end;

void paging_init(multiboot_uint32_t mmap_addr, multiboot_uint32_t mmap_len)
{
	multiboot_memory_map_t* mmap = (void*) mmap_addr;
    //size_t kernel_size = (size_t) &_kernel_phys_end - (size_t) &_kernel_phys_start;

	memset((void*)page_bitmap, 0xFF, 32768*sizeof(size_t));	
	memset((void*)superpage_bitmap, 0, sizeof(size_t) * 32);

	size_t free_memory_count = 0; // in bytes

	size_t mapping_address = 0x01000000;
	
	while((size_t)mmap < (mmap_addr + mmap_len))
	{
		if(mmap->type != MULTIBOOT_MEMORY_AVAILABLE)
		{
			res_map[res_counter++] = mmap;
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

		free_map[free_counter++] = mmap;

		volatile size_t it = mmap->addr; /* without volatile this whole loop gets skipped */
		
		free_memory_count += mmap->len;

		while(it < (mmap->addr+mmap->len))
		{
			if((mapping_address >= (size_t)&_kernel_virt_start && mapping_address <= (size_t)&_kernel_virt_end) ||
			(mapping_address <= (size_t)&_kernel_virt_start && (mapping_address + 0x1000) >= (size_t)&_kernel_virt_start) ||
			(mapping_address < (size_t)&_kernel_virt_end && (mapping_address + 0x1000) >= (size_t)&_kernel_virt_end))
			{
				mapping_address += 0x1000;
				continue;
			}

			if((it >= (size_t)&_kernel_phys_start && it <= (size_t)&_kernel_phys_end) ||
				(it <= (size_t)&_kernel_phys_start && (it + 0x1000) >= (size_t)&_kernel_phys_start) ||
				(it < (size_t)&_kernel_phys_end && (it + 0x1000) >= (size_t)&_kernel_phys_end) ||
				(it < 0x01000000))
			{
				it += 0x1000;
				continue;
			}

			map_page(it, mapping_address);

			const size_t current_page = mapping_address/0x1000;

			if(page_bitmap[current_page/32] == 0xFFFFFFFF)
			{
				--superpage_bitmap[current_page/0x1000];
			}

			page_bitmap[current_page / 32] &= ~(1 << (current_page%32));


			it += 0x1000;
			mapping_address += 0x1000;

			if(mapping_address < 0x1000000)
			{
				printf("Memory truncated\n");
				break;
			}
		}
		
		end:
		mmap = (multiboot_memory_map_t*) ((size_t)mmap + mmap->size + sizeof(mmap->size));
	}
}

void map_page(uint32_t physical, uint32_t virtual)
{
	page_directory[virtual >> 22] |= 3;
    page_table[virtual >> 22][(virtual >> 12) & 0x3ff] = (physical & ~0xFFF) | 3;
	flush_tlb();
}

inline uint32_t get_physaddr(uint32_t virtual)
{
    return (page_table[virtual >> 22][(virtual >> 12) & 0x3ff] & ~0xFFF) + (virtual & 0xfff);
}
