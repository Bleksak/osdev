#pragma once

#include <stdbool.h>

enum CPUID_FEATURES
{
    CPUID_FEAT_FPU          = 1ULL << 0,
    CPUID_FEAT_VME          = 1ULL << 1,  
    CPUID_FEAT_DE           = 1ULL << 2,  
    CPUID_FEAT_PSE          = 1ULL << 3,  
    CPUID_FEAT_TSC          = 1ULL << 4,  
    CPUID_FEAT_MSR          = 1ULL << 5,  
    CPUID_FEAT_PAE          = 1ULL << 6,  
    CPUID_FEAT_MCE          = 1ULL << 7,  
    CPUID_FEAT_CX8          = 1ULL << 8,  
    CPUID_FEAT_APIC         = 1ULL << 9,  
    CPUID_FEAT_SEP          = 1ULL << 11, 
    CPUID_FEAT_MTRR         = 1ULL << 12, 
    CPUID_FEAT_PGE          = 1ULL << 13, 
    CPUID_FEAT_MCA          = 1ULL << 14, 
    CPUID_FEAT_CMOV         = 1ULL << 15, 
    CPUID_FEAT_PAT          = 1ULL << 16, 
    CPUID_FEAT_PSE36        = 1ULL << 17, 
    CPUID_FEAT_PSN          = 1ULL << 18, 
    CPUID_FEAT_CLF          = 1ULL << 19, 
    CPUID_FEAT_DTES         = 1ULL << 21, 
    CPUID_FEAT_ACPI         = 1ULL << 22, 
    CPUID_FEAT_MMX          = 1ULL << 23, 
    CPUID_FEAT_FXSR         = 1ULL << 24, 
    CPUID_FEAT_SSE          = 1ULL << 25, 
    CPUID_FEAT_SSE2         = 1ULL << 26, 
    CPUID_FEAT_SS           = 1ULL << 27, 
    CPUID_FEAT_HTT          = 1ULL << 28, 
    CPUID_FEAT_TM1          = 1ULL << 29, 
    CPUID_FEAT_IA64         = 1ULL << 30,
    CPUID_FEAT_PBE          = 1ULL << 31,

    CPUID_FEAT_SSE3         = 1ULL << 32, 
    CPUID_FEAT_PCLMUL       = 1ULL << 33,
    CPUID_FEAT_DTES64       = 1ULL << 34,
    CPUID_FEAT_MONITOR      = 1ULL << 35,
    CPUID_FEAT_DS_CPL       = 1ULL << 36,  
    CPUID_FEAT_VMX          = 1ULL << 37,  
    CPUID_FEAT_SMX          = 1ULL << 38,  
    CPUID_FEAT_EST          = 1ULL << 39,  
    CPUID_FEAT_TM2          = 1ULL << 40,  
    CPUID_FEAT_SSSE3        = 1ULL << 41,  
    CPUID_FEAT_CID          = 1ULL << 42,
    CPUID_FEAT_FMA          = 1ULL << 43,
    CPUID_FEAT_CX16         = 1ULL << 44, 
    CPUID_FEAT_ETPRD        = 1ULL << 45, 
    CPUID_FEAT_PDCM         = 1ULL << 46, 
    CPUID_FEAT_PCIDE        = 1ULL << 47, 
    CPUID_FEAT_DCA          = 1ULL << 49, 
    CPUID_FEAT_SSE4_1       = 1ULL << 50, 
    CPUID_FEAT_SSE4_2       = 1ULL << 51, 
    CPUID_FEAT_x2APIC       = 1ULL << 52, 
    CPUID_FEAT_MOVBE        = 1ULL << 53, 
    CPUID_FEAT_POPCNT       = 1ULL << 54, 
    CPUID_FEAT_AES          = 1ULL << 55, 
    CPUID_FEAT_XSAVE        = 1ULL << 56, 
    CPUID_FEAT_OSXSAVE      = 1ULL << 57, 
    CPUID_FEAT_AVX          = 1ULL << 58,
};

struct cpuid
{
    unsigned int max_cpuid;
    char cpu_brand_string[12];
    unsigned int cpu_signature;
    unsigned char brand_index;
    unsigned char clflush_line_size; // *8 clflsh feature flag
    unsigned char logical_cpu_count; // Hyper-Threading flag
    unsigned char local_apic_id;
    unsigned long long features;

    unsigned int maximum_cpuid_input_value_ext;
    unsigned char cpu_brand_string_ext[48];
} PACKED;

void cpu_init(void);

struct cpuid* get_cpu_info(void);

bool cpu_has_feature(enum CPUID_FEATURES feature);