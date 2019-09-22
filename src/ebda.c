#include "ebda.h"
#include "ebda.h"
#include "bda.h"
#include "assert.h"
#include "paging.h"

const struct ExtendedBiosDataArea* ebda;

void ebda_init(const uintptr_t ebda_ptr) {
    ebda = MAP_SIZE(ebda_ptr, sizeof(struct ExtendedBiosDataArea), Present);
    assert(ebda->ebda_size == 1 || ebda->ebda_size == 2);
}

const struct ExtendedBiosDataArea* ebda_get(void) {
    return ebda;
}
