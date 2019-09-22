#pragma once

struct Registers {
    unsigned int eax, ebx, ecx, edx, esi, edi, ebp, esp;
};

#define CPUID_FEAT_FPU          1ULL << 0
#define CPUID_FEAT_VME          1ULL << 1
#define CPUID_FEAT_DE           1ULL << 2
#define CPUID_FEAT_PSE          1ULL << 3
#define CPUID_FEAT_TSC          1ULL << 4
#define CPUID_FEAT_MSR          1ULL << 5
#define CPUID_FEAT_PAE          1ULL << 6
#define CPUID_FEAT_MCE          1ULL << 7
#define CPUID_FEAT_CX8          1ULL << 8
#define CPUID_FEAT_APIC         1ULL << 9
#define CPUID_FEAT_SEP          1ULL << 11
#define CPUID_FEAT_MTRR         1ULL << 12
#define CPUID_FEAT_PGE          1ULL << 13
#define CPUID_FEAT_MCA          1ULL << 14
#define CPUID_FEAT_CMOV         1ULL << 15
#define CPUID_FEAT_PAT          1ULL << 16
#define CPUID_FEAT_PSE36        1ULL << 17
#define CPUID_FEAT_PSN          1ULL << 18
#define CPUID_FEAT_CLF          1ULL << 19
#define CPUID_FEAT_DTES         1ULL << 21
#define CPUID_FEAT_ACPI         1ULL << 22
#define CPUID_FEAT_MMX          1ULL << 23
#define CPUID_FEAT_FXSR         1ULL << 24
#define CPUID_FEAT_SSE          1ULL << 25
#define CPUID_FEAT_SSE2         1ULL << 26
#define CPUID_FEAT_SS           1ULL << 27
#define CPUID_FEAT_HTT          1ULL << 28
#define CPUID_FEAT_TM1          1ULL << 29
#define CPUID_FEAT_IA64         1ULL << 30
#define CPUID_FEAT_PBE          1ULL << 31

#define CPUID_FEAT_SSE3         1ULL << 32
#define CPUID_FEAT_PCLMUL       1ULL << 33
#define CPUID_FEAT_DTES64       1ULL << 34
#define CPUID_FEAT_MONITOR      1ULL << 35
#define CPUID_FEAT_DS_CPL       1ULL << 36
#define CPUID_FEAT_VMX          1ULL << 37
#define CPUID_FEAT_SMX          1ULL << 38
#define CPUID_FEAT_EST          1ULL << 39
#define CPUID_FEAT_TM2          1ULL << 40
#define CPUID_FEAT_SSSE3        1ULL << 41
#define CPUID_FEAT_CID          1ULL << 42
#define CPUID_FEAT_FMA          1ULL << 43
#define CPUID_FEAT_CX16         1ULL << 44
#define CPUID_FEAT_ETPRD        1ULL << 45
#define CPUID_FEAT_PDCM         1ULL << 46
#define CPUID_FEAT_PCIDE        1ULL << 47
#define CPUID_FEAT_DCA          1ULL << 49
#define CPUID_FEAT_SSE4_1       1ULL << 50
#define CPUID_FEAT_SSE4_2       1ULL << 51
#define CPUID_FEAT_x2APIC       1ULL << 52
#define CPUID_FEAT_MOVBE        1ULL << 53
#define CPUID_FEAT_POPCNT       1ULL << 54
#define CPUID_FEAT_AES          1ULL << 55
#define CPUID_FEAT_XSAVE        1ULL << 56
#define CPUID_FEAT_OSXSAVE      1ULL << 57
#define CPUID_FEAT_AVX          1ULL << 58

// Ext features

#define CPUID_EXT_FEAT_FSGSBASE 0
#define CPUID_EXT_FEAT_IA32_TSC_ADJUST 1
#define CPUID_EXT_FEAT_SGX 2
#define CPUID_EXT_FEAT_BMI1 3
#define CPUID_EXT_FEAT_HLE 4
#define CPUID_EXT_FEAT_AVX2 5
#define CPUID_EXT_FEAT_SMEP 7
#define CPUID_EXT_FEAT_BMI2 8
#define CPUID_EXT_FEAT_ERMS 9
#define CPUID_EXT_FEAT_INVPCID 10
#define CPUID_EXT_FEAT_RTM 11
#define CPUID_EXT_FEAT_PQM 12
#define CPUID_EXT_FEAT_FPU_CS_DS_DEPRECATED 13
#define CPUID_EXT_FEAT_MPX 14
#define CPUID_EXT_FEAT_PQE 15
#define CPUID_EXT_FEAT_AVX512F 16
#define CPUID_EXT_FEAT_AVX512DQ 17
#define CPUID_EXT_FEAT_RDSEED 18
#define CPUID_EXT_FEAT_ADX 19
#define CPUID_EXT_FEAT_SMAP 20
#define CPUID_EXT_FEAT_AVX512IFMA 21
#define CPUID_EXT_FEAT_PCOMMIT 22
#define CPUID_EXT_FEAT_CLFLUSHOPT 23
#define CPUID_EXT_FEAT_CLWB 24
#define CPUID_EXT_FEAT_INTEL_PT 25
#define CPUID_EXT_FEAT_AVX512PF 26
#define CPUID_EXT_FEAT_AVX512ER 27
#define CPUID_EXT_FEAT_AVX512CD 28
#define CPUID_EXT_FEAT_SHA 29
#define CPUID_EXT_FEAT_AVX512BW 30
#define CPUID_EXT_FEAT_AVX512VL 31

 // EDX:

#define CPUID_EXT_FEAT_AVX512_4VNNIW 34
#define CPUID_EXT_FEAT_AVX512_4FMAPS 35
#define CPUID_EXT_FEAT_PCONFIG 50
#define CPUID_EXT_FEAT_SPEC_CTRL 58
#define CPUID_EXT_FEAT_IBC 59
#define IA32_ARCH_CAPABILITIES_MSR 61
 
 // ECX:

#define CPUID_EXT_FEAT_PREFETCHWT1 64
#define CPUID_EXT_FEAT_AVX512VBMI 65
#define CPUID_EXT_FEAT_UMIP 66
#define CPUID_EXT_FEAT_PKU 67
#define CPUID_EXT_FEAT_OSPKE 68
#define CPUID_EXT_FEAT_AVX512VBMI2 70
#define CPUID_EXT_FEAT_GFNI 72
#define CPUID_EXT_FEAT_VAES 73
#define CPUID_EXT_FEAT_VPCLMULQDQ 74
#define CPUID_EXT_FEAT_AVX512VNNI 75
#define CPUID_EXT_FEAT_AVX512BITALG 76
#define CPUID_EXT_FEAT_AVX512VPOPCNTDQ 78
#define CPUID_EXT_FEAT_MAWAU 80
#define CPUID_EXT_FEAT_RDPID 81
#define CPUID_EXT_FEAT_SGX_LC 89

typedef struct Registers Registers;

#include "../std.h"
#include "../attributes.h"

#include "ext/fpu.h"
#include "ext/sse.h"
#include "ext/xsave.h"
#include "ext/avx.h"

struct CPUID {
    uint32_t max_cpuid;
    char cpu_brand_string[12];
    uint32_t cpu_signature;
    uint8_t brand_index;
    uint8_t clflush_line_size; // *8 clflsh feature flag
    uint8_t logical_cpu_count; // Hyper-Threading flag
    uint8_t local_apic_id;
    uint32_t features[2];
    uint32_t ext_features[3];

    uint32_t maximum_cpuid_input_value_ext;
    char cpu_brand_string_ext[48];
} PACKED;

void cpu_init(void);

struct CPUID* get_cpu_info(void);


bool cpu_has_feature(uint64_t feature);
bool cpu_has_ext_feature(uint64_t feature);
