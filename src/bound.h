#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "attributes.h"

FUNCTIONAL bool bound(uintptr_t start, uintptr_t end, uintptr_t start_addr, uintptr_t end_addr);
FUNCTIONAL bool bound_s(uintptr_t start, uintptr_t end, uintptr_t addr);