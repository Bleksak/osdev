#include "bda.h"
#include "ebda.h"
#include "paging.h"

#include "console.h"

#define BDA_OFFSET 0

const struct BiosDataArea* bda;

void bda_init(void) {
    const uintptr_t virtual_offset = mem_offset_get();
    map_page(0, virtual_offset, Present);
    mem_offset_set(virtual_offset + 0x1000);

    bda = (const struct BiosDataArea*)virtual_offset;

    ebda_init(bda->ExtendedBDAPtr * 0x10);
}

const struct BiosDataArea* bda_get(void) {
    return bda;
}
