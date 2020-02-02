#include "pheap.h"
#include "console.h"

#include "cpu/cpuid.h"

#include <memory.h>
#include <paging.h>

// 32768 pages per 1 superpage
// 1024 pages  per 1 bit in superpage

size_t page_bitmap[MAX_PAGE];
size_t superpage_bitmap[MAX_SUPERPAGE];

static void page_mark(size_t page) {
	page_set_flags(page, Present | ReadWrite);
	page_bitmap[page >> 5] |= 1 << (page & 0x1F);
}

void page_unmark(size_t page) {
	page_unset_flags(page, Present | ReadWrite);
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

static size_t superpage_find_free(size_t start) { // returns page or 0xfffffff
	while(superpage_bitmap[start] == 0xFFFFFFFF) {
		if(++start >= MAX_SUPERPAGE) {
			return 0xFFFFFFFF;
		}
	}

	const size_t value = ~superpage_bitmap[start];
	
	size_t bit = 0;

	if(cpu_has_ext_feature(CPUID_EXT_FEAT_BMI1)) {
		__asm__ volatile("blsi %1, %0" : "=r"(bit) : "r"(value)); // supported in qemu
	} else {
		bit = value & (-value);
	}

	return (start << 15) + ((bit - 1) << 10);
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

static size_t page_find_free_start(size_t start, size_t count) {

	const size_t to_superpage = start << 15;

	size_t page_index = superpage_find_free(to_superpage);

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
	const size_t page = page_find_free(count);

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


void* repalloc(void* addr, const size_t oldcount, const size_t newcount) {
	const size_t page = ((uintptr_t) (addr) >> 12);

	if(!addr || !oldcount) {
		return palloc(newcount);
	}

	if(newcount > oldcount) { // normal scenario
		const size_t first_check = page + oldcount;
		const size_t to_allocate = newcount - oldcount;
		const size_t alloc_page = page_find_free_start(first_check, to_allocate);

		if(alloc_page == first_check) {
			// we gucci, allocate this

			for(size_t i = alloc_page; i < (alloc_page + to_allocate); ++i) {
				page_mark(i);
				if(page_bitmap[i >> 5] == 0xFFFFFFFF) {
					superpage_mark(i);
				}
			}

			return addr;
		}
		
		void* new_alloc = palloc(newcount);
		
		if(!new_alloc) {
			return 0; // if we couldn't allocate, return
		}
		
		(void) memcpy(new_alloc, addr, oldcount * PAGE_SIZE);
		pfree(addr, oldcount);

		return new_alloc;
	}

	// otherwise we free

	const size_t to_free = oldcount - newcount;
	const size_t free_begin = page + to_free;
	void* free_addr = (void*)(free_begin << 12);

	// value less than addr can never occur since newcount is unsigned
	// only if oldcount is greater than actual oldcount

	pfree(free_addr, to_free);
	return addr;
}
