#include "xsave.h"

#include "cpu/cpuid.h"
#include "cpu/cr.h"

#include "console.h"

void xsave_enable(void) {
    printf("Enabling OSXSAVE: ");
    
    if(cpu_has_feature(CPUID_FEAT_OSXSAVE)) {
        cr4_set(CR4_OSXSAVE);
        return printf_colored(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "Success\n");
    }

    printf_colored(VGA_COLOR_RED, VGA_COLOR_BLACK, "Failed\n");
}
