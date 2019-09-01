#include "pheap.h"
#include "console.h"

#define MAX_ALLOC_COUNT 1048576 // 4096 * 
#define MAX_SUPERPAGE 32
#define MAX_PAGE 32768

extern size_t page_bitmap[MAX_PAGE];
extern size_t superpage_bitmap[MAX_SUPERPAGE];

static void* page_to_address(size_t page) {
	return (void*) (page * 4096);
}

// static size_t superpage_to_page(size_t superpage) {
// 	return superpage * 0x8000;
// }

// static size_t superpage_find_free(size_t start) {
// 	while(superpage_bitmap[start] == 0xFFFFFFFF) {
// 		if(++start >= MAX_SUPERPAGE) {
// 			return MAX_SUPERPAGE;
// 		}
// 	}
	
// 	return start;
// }

static size_t page_find_free(size_t count) {
	// size_t superpage_index = superpage_find_free(0);

	// if(superpage_index == MAX_SUPERPAGE) {
	// 	return 0;
	// }

	// size_t page_index = superpage_to_page(superpage_index);

	size_t page_index = 0;

	while(page_bitmap[page_index] == 0xFFFFFFFF) {
		if((++page_index) % 0x8000 == 0) {
			// superpage_index = superpage_find_free(superpage_index + 1);
			// if(superpage_index == MAX_SUPERPAGE) {
			// 	return 0;
			// }

			if(page_index == MAX_PAGE) {
				return 0;
			}
		}
	}

	size_t current_page = page_index;
	size_t current_count = 0;
	size_t current_start = 0;

	while(true) {
		for(size_t i = 0; i < 32; ++i) { // loop through all the bits in the entry
			if((page_bitmap[current_page] & (1 << i)) == 0) { // need to check if the current bit is 0
				if(++current_count == count) { // if it is we increment the counter
					return (page_index << 5) + current_start;
				}
			}
			else { // otherwise we reset and start at current position, because we can't fit "count" bits
				current_start = i;
				current_count = 0;
			}
		}

		if(++current_page == MAX_PAGE) {
			return 0;
		}
	}
}

#if 0

void* palloc(size_t count)
{
	if(!count || count > MAX_ALLOC_COUNT)
		return 0;
	
	size_t current_count = 0;
	size_t start = 0;
	size_t real_start = 0;
	
	while(superpage_bitmap[real_start] == 0xFFFFFFFF)
	{
		if(++real_start > 31)
		{
			return 0;
		}
	}

	real_start *= 32;

	while(page_bitmap[real_start] == 0xFFFFFFFF)
	{
		if(++real_start > 32767)
		{
			return 0;
		}
	}
	
	start = real_start * 32;

	for(size_t i = real_start; i < 32768; ++i)
	{
		const size_t val = page_bitmap[i];

		for(size_t j = 0; j < 32; ++j)
		{
			if(val & (1 << j))
			{
				current_count = 0;
				start = i * 32 + j + 1;
			}
			else if(++current_count == count)
				goto done;
		}
	}
	
done:
	if(current_count != count)
	{
		return 0;
	}

	for(size_t i = start; i < (start + count); ++i)
	{
		page_bitmap[i/32] |= 1 << (i % 32);
		if(page_bitmap[i/32] == 0xFFFFFFFF)
			++superpage_bitmap[i/1024];
	}

	return (void*) (start * 0x1000);
}

#endif

void* palloc(size_t count) {
	// 2

	size_t nr = page_find_free(count); // 32
	
	// printf("NR: %u\n", nr);

	if(!nr) {
		return 0;
	}

	size_t base_offset = nr % 32; // 0
	size_t base_page = nr / 32; // 1
										// 0 < 2
	for(size_t current_count = 0; current_count < count; ++current_count) {
		// printf("Bitmap dump PRE: %x\n", page_bitmap[base_page + ((current_count + base_offset) >> 5)]);
		page_bitmap[base_page + ((current_count + base_offset) >> 5)] |= 1 << ((current_count + base_offset) % 32);

		// printf("Bitmap dump POST: %x\n", page_bitmap[base_page + ((current_count + base_offset) >> 5)]);

		if(page_bitmap[base_page + ((current_count + base_offset) >> 5)] == 0xFFFFFFFF) {
			// superpage_bitmap[(base_page + current_count >> 5) >> 10]++; WRONG
		}
	}

	return page_to_address(nr);
}

void pfree(void* addr, size_t count)
{
	const size_t start = ((size_t) addr/0x1000);
	const size_t end = start + count;

	for(size_t i = start; i < end; ++i)
	{
		if(page_bitmap[i/32] == 0xFFFFFFFF)
			--superpage_bitmap[i/1024];

		page_bitmap[i/32] &= ~(1 << (i % 32));
	}
}
