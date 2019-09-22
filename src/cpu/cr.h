#pragma once

#include "../std.h"

#define MSR_EFER 0xC0000080
#define MSR_XSS  0x0DA0
#define MSR_APIC 0x1B

enum CR0 {
    CR0_PE = 1 << 0,  // Protected mode enable
    CR0_MP = 1 << 1,  // Monitor co-processor
    CR0_EM = 1 << 2,  // Emulation
    CR0_TS = 1 << 3,  // Task switched
    CR0_ET = 1 << 4,  // Extension type
    CR0_NE = 1 << 5,  // Numeric error
    CR0_WP = 1 << 16, // Write protect
    CR0_AM = 1 << 18, // Alignment mask
    CR0_NW = 1 << 29, // Not-write through
    CR0_CD = 1 << 30, // Cache disable
    CR0_PG = 1 << 31, // Paging enable
};

enum CR4 {
    CR4_VME        = 1 << 0, // Virtual 8086 mode
    CR4_PVI        = 1 << 1, // Protected mode virtual interrupts
    CR4_TSD        = 1 << 2,
    CR4_DE         = 1 << 3,
    CR4_PSE        = 1 << 4,
    CR4_PAE        = 1 << 5,
    CR4_MCE        = 1 << 6,
    CR4_PGE        = 1 << 7,
    CR4_PCE        = 1 << 8,
    CR4_OXFXSR     = 1 << 9,
    CR4_OSXMMEXCPT = 1 << 10,
    CR4_UMIP       = 1 << 11,
    CR4_LA57       = 1 << 12,
    CR4_VMXE       = 1 << 13,
    CR4_SMXE       = 1 << 14,
    CR4_FSGBASE    = 1 << 16,
    CR4_PCIDE      = 1 << 17,
    CR4_OSXSAVE    = 1 << 18,
    CR4_SMEP       = 1 << 20,
    CR4_SMAP       = 1 << 21,
    CR4_PKE        = 1 << 22,
};

enum XCR0 {
    XCR0_FPU = 1 << 0,
    XCR0_XSAVE = 1 << 1,
    XCR0_AVX = 1 << 2,
    XCR0_BNDREG = 1 << 3,
    XCR0_BNDCSR = 1 << 4,
    XCR0_AVX512_OPMASK = 1 << 5,
    XCR0_AVX512_ZMM = 1 << 6,
    XCR0_AVX512_ZMM_HI = 1 << 7,
    XCR0_PKRU = 1 << 9,
    XCR0_DAAX = 1 << 10,
};

enum XSS {
    XSS_PT = 1 << 8,
    XSS_HDC = 1 << 13,
};

enum EFER {
    EFER_SCE = 1 << 0,
    EFER_LME = 1 << 8,
    EFER_LMA = 1 << 10,
    EFER_NXE = 1 << 11,
    EFER_SVME = 1 << 12,
    EFER_LMSLE = 1 << 13,
    EFER_FFXSR = 1 << 14,
    EFER_TCE = 1 << 15,
};

uint32_t cr0_get(uint32_t mask);
void cr0_set(uint32_t mask);
void cr0_clear(uint32_t mask);

uint32_t cr3_get(void);
void cr3_set(uint32_t);

uint32_t cr4_get(uint32_t mask);
void cr4_set(uint32_t mask);

uint64_t rdmsr(uint32_t ecx);
void wrmsr(uint32_t ecx, uint64_t value);

uint64_t efer_get(uint64_t mask);
void efer_set(uint64_t mask);

uint64_t xss_get(uint64_t mask);
void xss_set(uint64_t mask);

uint64_t xcr0_get(uint64_t mask);
void xcr0_set(uint64_t mask);
void xcr0_clear(uint64_t mask);
