#include "fpu.h"
#include "cpu/cpuid.h"
#include "cpu/cr.h"

bool fpu_check(void);

void fpu_enable(void)
{
    if(cpu_has_feature(CPUID_FEAT_FPU) && cr0_get(CR0_EM | CR0_ET) == CR0_EM | CR0_ET)
    {
        fpu_enable();
    }
}