#include "mheap.h"
#include "pheap.h"
#include "align.h"
#include "memory.h"

typedef struct alloc {
	struct alloc* next;
	struct alloc* prev;
	struct alloc* prev_phys;

	unsigned int free;
	size_t size;
} __attribute__((aligned(32))) alloc_t;

size_t kernel_mem = 0;

/*
	realloc can give different location while freeing the current
	if realloc cannot give mem, dont free, return 0,
	if realloc is successful, return new location (or old resized)
*/

alloc_t* free_allocs = 0, *used_allocs = 0;
static size_t free_memory = 0;
static size_t used_memory = 4096;

size_t heap_get_free_mem(void) {
	return free_memory;
}

void heap_init(void) {
	free_allocs = palloc(1);
	for(size_t i = 0; i < MAX_PAGE << 5; ++i) {
		if(!page_get_value(i)) {
			free_memory += 4096;
		}
	}
}

static void* alloc(alloc_t* block, size_t size) {
	if((block->size - size) <= sizeof(alloc_t)) {
		/*
			alloc the whole block
		*/

		block->free = 0;
		block->next = used_allocs;
		block->prev = 0;
		used_allocs = block;

		free_memory -= block->size;
		used_memory += block->size;

		return (void*)(block+1);
	}
	/*
		Make sure size is 32 bytes aligned
	*/

	alloc_t* new = (alloc_t*)((size_t)block + size);

	if(block->prev) {
		block->prev->next = block->next;
	}
	
	if(block->next){
		block->next->prev = block->prev;
	}

	new->prev_phys = block;

	new->size = block->size - size;

	block->size = size;
	block->free = false;
	block->next = used_allocs;
	block->prev = 0;
	used_allocs = block;

	new->free = true;
	new->next = free_allocs;
	new->prev = 0;
	new->prev_phys = block;

	free_allocs = new;

	free_memory -= size;
	used_memory += size;

	return (void*)(block + 1);
}

void* malloc(size_t size) {
	if(!size)
		return 0;
	
	size = align(size + sizeof(alloc_t), sizeof(alloc_t));
	
	alloc_t* current_block = free_allocs;

	while(current_block) {
		if(current_block->size >= size) {
			return alloc(current_block, size);
		}

		current_block = current_block->next;
	}

	/*
		size of nodes is not sufficient, allocate a new one
	*/
	
	const size_t aligned = align(size, 4096);
	const size_t pages_needed = aligned / 4096;

	alloc_t* new_block = palloc(pages_needed);

	if(!new_block) {
		return 0;

	}
	
	new_block->next = 0;
	new_block->prev = 0;
	new_block->size = aligned;
	new_block->free = true;
	new_block->prev_phys = 0;

	return alloc(new_block, size);
}

void free(void* address) {
	if(!address) {
		return;
	}
	
	alloc_t* block = ((alloc_t*)address - 1);

	if(block->free) {
		return;
	}
	
	used_memory -= block->size;
	free_memory += block->size;

	/*
		block + block->size = next phys node
		how to merge backwards?	
	*/

	alloc_t* current = block;

	size_t merge_size = 0;

	do {
		merge_size += current->size;
		if(current->next)
			current->next->prev = current->prev;

		if(current->prev)
			current->prev->next = current->next;
		
		current = (alloc_t*)((size_t)current + current->size);
	} while((current->size % 32 == 0) && current->free == 1 && (current->next || current->prev));

	alloc_t* start = block;

	if(block->prev_phys && block->prev_phys->free) {
		start = block->prev_phys;

		while(true) {
			merge_size += start->size;

			if(!start->prev_phys || !start->prev_phys->free)
				break;
			
			start = start->prev_phys;
		}
	}

	size_t aligned = align((uintptr_t)start, 4096);
	size_t difference = aligned - (uintptr_t)start;

	size_t start_merge_size = merge_size;

	if(merge_size >= 4096) {
		if(difference) {
			merge_size -= difference;
			start = (alloc_t*)aligned;

			if(merge_size < 4096) {
				merge_size = start_merge_size;
				goto free_fail;
			}
		}

		/*
			pfree the pages
		*/
		size_t pages = merge_size / 4096;
		pfree(start, pages);

		start += (size_t)(pages * 4096);
		merge_size %= 4096;
	}

	free_fail:

	/*
		stop merging, put one merged block into free_allocs
	*/
	
	if(!merge_size) {
		/*
			merge size is 0, means all pages were deallocated and we need to pop the block out
		*/

		if(block->next)
			block->next->prev = block->prev;
	
		if(block->prev)
			block->prev->next = block->next;
		
		memset(block, 0, sizeof(alloc_t));

		return;
	}
	
	start->size = merge_size;
	start->free = true;
	
	start->next = free_allocs;
	start->prev = 0;

	free_allocs = start;
}

static void shrink_to_fit(alloc_t* block, size_t size) {
	alloc_t* new = (alloc_t*)((uintptr_t) block + size);
	new->size = block->size - size;
	new->free = false;
	
	new->prev = 0;
	new->next = used_allocs;

	new->prev_phys = block;
	
	used_allocs = new;
	block->size = size;

	free(new+1);
}

void* calloc(size_t count, size_t size) {
	void* addr = malloc(count*size);
	
	if(addr) {
		memset(addr, 0, count * size);
	}

	return addr;
}

void* realloc(void* base, size_t size) {
	if(!size) {
		free(base);
		return 0;
	}

	if(!base) {
		return malloc(size);
	}

	alloc_t* block = ((alloc_t*)base - 1);

	const size_t new_size = align(size + sizeof(alloc_t), sizeof(alloc_t));

	if(block->size == new_size) {
		return base;
	}

	if(block->size > new_size) {
		shrink_to_fit(block, new_size);
		return base;
	}

	alloc_t* next_alloc = (alloc_t*) ((uintptr_t)block + block->size);
	const long long new_alloc_size = (block->size + next_alloc->size) - new_size;

	if(next_alloc->free && new_alloc_size >= 0) {
		if(next_alloc->next) {
			next_alloc->next->prev = next_alloc->prev;
		}

		if(next_alloc->prev) {
			next_alloc->prev->next = next_alloc->next;
		}

		if(new_alloc_size <= (2 * sizeof(alloc_t))) {
			block->size += new_alloc_size;
			return (void*) (block + 1);
		}

		block->size = new_size;

		alloc_t* new_created_malloc = (alloc_t*) ((uintptr_t)block + (uintptr_t)block->size);
		new_created_malloc->size = new_alloc_size;
		new_created_malloc->free = true;
		new_created_malloc->next = free_allocs;
		new_created_malloc->prev = 0;
		free_allocs->prev = new_created_malloc;
		free_allocs = new_created_malloc;

		return (void*) (block + 1);
	}

	alloc_t* new_block = malloc(new_size);
	
	if(!new_block) {
		return 0;
	}

	memcpy(new_block, block, block->size);

	free(block);

	return (void*) (new_block + 1);
}
