#include "pheap.h"
#include "console.h"

#include "cpu/cpuid.h"

// 32768 pages per 1 superpage
// 1024 pages  per 1 bit in superpage

size_t page_bitmap[MAX_PAGE];
size_t superpage_bitmap[MAX_SUPERPAGE];

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

size_t page_get_value(size_t page) {
	return page_bitmap[page >> 5] & (1 << (page & 0x1F));
}

static size_t superpage_find_free(void) { // returns page or 0xfffffff
	size_t start = 0;

	while(superpage_bitmap[start] == 0xFFFFFFFF) {
		if(++start >= MAX_SUPERPAGE) {
			return 0xFFFFFFFF;
		}
	}

	const size_t value = ~superpage_bitmap[start];
	
	size_t bit = 0;

	if(cpu_has_ext_feature(CPUID_EXT_FEAT_BMI1)) {
		__asm__ volatile("blsi %1, %0" : "=r"(bit) : "r"(value));
	} else {
		bit = value & (-value);
	}

	return (start << 15) + ((bit - 1) << 10);
}

static size_t page_find_free(size_t count) {
	size_t page_index = superpage_find_free();

	if(page_index == 0xFFFFFFFF) {
		return 0;
	}

	while(page_bitmap[page_index] == 0xFFFFFFFF) {
		if((++page_index) == MAX_PAGE) {
			return 0;
		}
	}

	
	size_t current_count = 0;
	size_t start_page = page_index << 5;

	for(size_t current_page = start_page; current_page < (MAX_PAGE << 5); ++current_page) {
		if(page_get_value(current_page)) {
			start_page = current_page + 1;
			current_count = 0;
			continue;
		}

		if(++current_count == count) {
			return start_page;
		}
	}

	return 0;
}

void* palloc(const size_t count) {
	size_t page = page_find_free(count);

	if(!page) {
		return 0;
	}

	for(size_t i = page; i < (page + count); ++i) {
		page_mark(i);
		if(page_bitmap[i >> 5] == 0xFFFFFFFF) {
			superpage_mark(i);
		}
	}

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
