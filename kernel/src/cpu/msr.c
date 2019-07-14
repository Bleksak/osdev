#include "cpu/msr.h"
#include "asm.h"

void cpu_get_msr(uint32_t msr, uint32_t* eax, uint32_t* edx)
{
    asm volatile("rdmsr" : "=a"(*eax), "=d"(*edx) : "c"(msr));
}

void cpu_set_msr(uint32_t msr, uint32_t eax, uint32_t edx)
{
    asm volatile("wrmsr" :: "a"(eax), "d"(edx), "c"(msr));
}
