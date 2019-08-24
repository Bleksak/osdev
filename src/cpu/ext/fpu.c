#include "fpu.h"
#include "../cpuid.h"
#include "../cr.h"

#include "../../console.h"

extern bool _fpu_enable(void);

void fpu_enable(void)
{
    if(cpu_has_feature(CPUID_FEAT_FPU) && cr0_get(CR0_EM | CR0_ET) == CR0_ET)
    {
        printf("Enabling FPU:");
        cr0_clear(CR0_EM | CR0_TS);

        if(_fpu_enable())
        {
            printf_colored(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "Success\n");
        }
        else printf_colored(VGA_COLOR_RED, VGA_COLOR_BLACK, "Failed\n");
    }
}