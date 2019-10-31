#include "bda.h"
#include "ebda.h"
#include "paging.h"

#include "console.h"

#define BDA_OFFSET 0

const BDA* bda;

void bda_init(void) {
    bda = map_size(0, sizeof(struct BiosDataArea), Present);
    ebda_init(bda->ebda_ptr * 0x10);
}

const BDA* bda_get(void) {
    return bda;
}
