#include "avx.h"
#include "../cr.h"
#include "../cpuid.h"
#include "../../console.h"

#include "../../std.h"

void avx_enable(void) {
    printf("Enabling AVX: ");

    if(cpu_has_feature(CPUID_FEAT_AVX | CPUID_FEAT_XSAVE)) {
        xcr0_set(XCR0_XSAVE);
        return printf_colored(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "Success\n");
    }
    
    printf_colored(VGA_COLOR_RED, VGA_COLOR_BLACK, "Failed");
}

void avx_512_enable(void) {
    printf("Enabling AVX-512: ");

    uint64_t flags[] = {
        CPUID_EXT_FEAT_AVX512_4FMAPS,
        CPUID_EXT_FEAT_AVX512_4VNNIW,
        CPUID_EXT_FEAT_AVX512BW ,
        CPUID_EXT_FEAT_AVX512CD ,
        CPUID_EXT_FEAT_AVX512DQ,
        CPUID_EXT_FEAT_AVX512ER,
        CPUID_EXT_FEAT_AVX512F,
        CPUID_EXT_FEAT_AVX512IFMA,
        CPUID_EXT_FEAT_AVX512PF,
        CPUID_EXT_FEAT_AVX512VL,
        CPUID_EXT_FEAT_AVX512VBMI,
        CPUID_EXT_FEAT_AVX512VBMI2,
        CPUID_EXT_FEAT_AVX512VNNI,
        CPUID_EXT_FEAT_AVX512BITALG,
        CPUID_EXT_FEAT_AVX512VPOPCNTDQ,
    };


    for (uint32_t i = 0; i < sizeof(flags)/sizeof(flags[0]); ++i) {
        if(cpu_has_ext_feature(flags[i])) {
            xcr0_set(XCR0_AVX512_OPMASK | XCR0_AVX512_ZMM | XCR0_AVX512_ZMM_HI);
            return printf_colored(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "Success\n");
        }
    }

    printf_colored(VGA_COLOR_RED, VGA_COLOR_BLACK, "Failed\n");
}