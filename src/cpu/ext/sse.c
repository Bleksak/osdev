#include "sse.h"
#include "../cpuid.h"
#include "../cr.h"
#include "../../console.h"

void sse_enable(void)
{
    printf("Enabling SSE: ");
    if(!cpu_has_feature(CPUID_FEAT_SSE))
    {
        printf_colored(VGA_COLOR_RED, VGA_COLOR_BLACK, "Failed\n");
        return;
    }

    cr0_set(CR0_EM); // enable emulation
    cr4_set(CR4_OXFXSR | CR4_OSXMMEXCPT); // enable sse -> fxsave + fxrstor | exceptions

    printf_colored(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "Success\n");
}

void fxsave(void* buffer) {
    __asm__ volatile("fxsave %0" :: "m"(buffer));
}

void fxrstor(void* buffer) {
    __asm__ volatile("fxrstor %0" :: "m"(buffer));
}
