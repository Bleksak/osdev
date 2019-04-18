#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "acpi.h"

char* AML_parse(struct RSDT* dsdt);

