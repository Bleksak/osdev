#include "pheap.h"
#include "memory.h"
#include "console.h"
#include "mheap.h"

extern size_t page_bitmap[32768];
extern size_t superpage_bitmap[32];

#if 0

size_t contains_mask(size_t value, size_t mask, size_t count)
{
	size_t start = 0;
	
	while(((value >> start) & mask) != mask)
	{
		if(++start > (31 - count))
		{
			return 32;
		}
	}

	return start;
}

void* pheap_alloc(size_t count)
{
	if(!count)
		return 0;
	
	if(count > 32) // we allocate 32 pages at most now
		return 0;

	size_t free_page = 0;

	size_t mask = (1 << (count-1)) ^ ~(-(1 << (count-1)));
	
	size_t mask_pos;
	
	while((mask_pos = contains_mask(~page_bitmap[free_page], mask, count)) > 31)
	{
		if(++free_page > 32767)
		{
			return 0;	
		}
	}

	mask <<= mask_pos;
	
	page_bitmap[free_page] |= mask;
	
	return (void*) (free_page * 0x20000 + mask_pos * 0x1000);
}
#else

void* palloc(size_t count)
{
	if(!count || count > 131072) 
		return 0;
	
	size_t current_count = 0, start = 0;
	
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
