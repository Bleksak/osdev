#include "pheap.h"
#include "console.h"

#include "cpu/cpuid.h"

#define MAX_ALLOC_COUNT 1048576 // 4096 * 
#define MAX_SUPERPAGE 32
#define MAX_PAGE 32768
// 32768 pages per 1 superpage
// 1024 pages  per 1 bit in superpage

extern size_t page_bitmap[MAX_PAGE];
extern size_t superpage_bitmap[MAX_SUPERPAGE];

static void page_mark(size_t page) {
	page_bitmap[page >> 5] |= 1 << (page & 0x1F);
}

void page_unmark(size_t page) {
	page_bitmap[page >> 5] &= ~(1 << (page & 0x1F));
}

static void superpage_mark(size_t page) {
	superpage_bitmap[page >> 15] |= 1 << (page & 0x3FF);
}

void superpage_unmark(size_t page) {
	superpage_bitmap[page >> 15] &= ~(1 << (page & 0x3FF));
}

static size_t superpage_find_free(size_t start) { // returns page or 0xfffffff
	while(superpage_bitmap[start] == 0xFFFFFFFF) {
		if(++start >= MAX_SUPERPAGE) {
			return 0xFFFFFFFF;
		}
	}

	const size_t value = superpage_bitmap[start];
	
	size_t bit = 0;

	if(cpu_has_ext_feature(CPUID_EXT_FEAT_BMI1)) {
		__asm__ volatile("blsi %1, %0" : "=r"(bit) : "r"(value));
	} else {
		bit = value & (-value);
	}

	return (start << 15) + (bit << 10);
}

static size_t page_find_free(size_t count) {
	size_t page_index = superpage_find_free(0);

	if(page_index == 0xFFFFFFFF) {
		return 0;
	}

	while(page_bitmap[page_index] == 0xFFFFFFFF) {
		if((++page_index) == MAX_PAGE) {
			return 0;
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

void* palloc(size_t count) {
	size_t page = page_find_free(count);

	if(!page) {
		return 0;
	}

	// size_t base_offset = nr % 32; // 0
	// size_t base_page = nr / 32; // 1

	for(size_t i = page; i < (page + count); ++i) {
		page_mark(i);
		superpage_mark(i);
	}

	// for(size_t current_count = 0; current_count < count; ++current_count) {
	// 	page_bitmap[base_page + ((current_count + base_offset) >> 5)] |= 1 << ((current_count + base_offset) % 32);
	// }

	return (void*) (page << 12);
}

void pfree(void* addr, size_t count) {
	const size_t start = ((size_t) addr >> 12);
	const size_t end = start + count;

	for(size_t i = start; i < end; ++i) {
		page_unmark(i);
		superpage_unmark(i);
	}
}
