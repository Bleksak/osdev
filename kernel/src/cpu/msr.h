#pragma once

#include <stdint.h>

#define MSR_APIC_BASE_REGISTER 0x1B
#define MSR_APIC_ENABLE 0x800


void cpu_get_msr(uint32_t msr, uint32_t* eax, uint32_t* edx);
void cpu_set_msr(uint32_t msr, uint32_t eax, uint32_t edx);
