#include "xsave.h"

#include "../cpuid.h"
#include "../cr.h"

#include "../../console.h"

void xsave_enable(void) {
    printf("Enabling XSAVE: ");
    
    if(cpu_has_ecx_feature(CPUID_FEAT_ECX_XSAVE)) {
        cr4_set(CR4_OSXSAVE);
        printf_colored(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "Success\n");
        return;
    }

    printf_colored(VGA_COLOR_RED, VGA_COLOR_BLACK, "Failed\n");
}
