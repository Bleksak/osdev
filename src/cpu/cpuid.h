#pragma once

#define    CPUID_FEAT_ECX_SSE3          1 << 0 
#define    CPUID_FEAT_ECX_PCLMUL        1 << 1
#define    CPUID_FEAT_ECX_DTES64        1 << 2
#define    CPUID_FEAT_ECX_MONITOR       1 << 3  
#define    CPUID_FEAT_ECX_DS_CPL        1 << 4  
#define    CPUID_FEAT_ECX_VMX           1 << 5  
#define    CPUID_FEAT_ECX_SMX           1 << 6  
#define    CPUID_FEAT_ECX_EST           1 << 7  
#define    CPUID_FEAT_ECX_TM2           1 << 8  
#define    CPUID_FEAT_ECX_SSSE3         1 << 9  
#define    CPUID_FEAT_ECX_CID           1 << 10
#define    CPUID_FEAT_ECX_FMA           1 << 12
#define    CPUID_FEAT_ECX_CX16          1 << 13 
#define    CPUID_FEAT_ECX_ETPRD         1 << 14 
#define    CPUID_FEAT_ECX_PDCM          1 << 15 
#define    CPUID_FEAT_ECX_PCIDE         1 << 17 
#define    CPUID_FEAT_ECX_DCA           1 << 18 
#define    CPUID_FEAT_ECX_SSE4_1        1 << 19 
#define    CPUID_FEAT_ECX_SSE4_2        1 << 20 
#define    CPUID_FEAT_ECX_x2APIC        1 << 21
#define    CPUID_FEAT_ECX_MOVBE         1 << 22
#define    CPUID_FEAT_ECX_POPCNT        1 << 23
#define    CPUID_FEAT_ECX_AES           1 << 25
#define    CPUID_FEAT_ECX_XSAVE         1 << 26
#define    CPUID_FEAT_ECX_OSXSAVE       1 << 27
#define    CPUID_FEAT_ECX_AVX           1 << 28

#define    CPUID_FEAT_EDX_FPU           1 << 0
#define    CPUID_FEAT_EDX_VME           1 << 1
#define    CPUID_FEAT_EDX_DE            1 << 2
#define    CPUID_FEAT_EDX_PSE           1 << 3
#define    CPUID_FEAT_EDX_TSC           1 << 4
#define    CPUID_FEAT_EDX_MSR           1 << 5
#define    CPUID_FEAT_EDX_PAE           1 << 6
#define    CPUID_FEAT_EDX_MCE           1 << 7
#define    CPUID_FEAT_EDX_CX8           1 << 8
#define    CPUID_FEAT_EDX_APIC          1 << 9
#define    CPUID_FEAT_EDX_SEP           1 << 11
#define    CPUID_FEAT_EDX_MTRR          1 << 12
#define    CPUID_FEAT_EDX_PGE           1 << 13
#define    CPUID_FEAT_EDX_MCA           1 << 14
#define    CPUID_FEAT_EDX_CMOV          1 << 15
#define    CPUID_FEAT_EDX_PAT           1 << 16
#define    CPUID_FEAT_EDX_PSE36         1 << 17
#define    CPUID_FEAT_EDX_PSN           1 << 18
#define    CPUID_FEAT_EDX_CLF           1 << 19
#define    CPUID_FEAT_EDX_DTES          1 << 21
#define    CPUID_FEAT_EDX_ACPI          1 << 22
#define    CPUID_FEAT_EDX_MMX           1 << 23
#define    CPUID_FEAT_EDX_FXSR          1 << 24
#define    CPUID_FEAT_EDX_SSE           1 << 25
#define    CPUID_FEAT_EDX_SSE2          1 << 26
#define    CPUID_FEAT_EDX_SS            1 << 27
#define    CPUID_FEAT_EDX_HTT           1 << 28
#define    CPUID_FEAT_EDX_TM1           1 << 29
#define    CPUID_FEAT_EDX_IA64          1 << 30
#define    CPUID_FEAT_EDX_PBE           1 << 31

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

#include <stdint.h>
#include <stdbool.h>

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
    struct {
        uint32_t ecx, edx;
    } features;

    uint32_t ext_features[3];

    uint32_t maximum_cpuid_input_value_ext;
    char cpu_brand_string_ext[48];
} __attribute__((packed));

void cpu_init(struct CPUID* id);

bool cpu_has_edx_feature(uint32_t feature);
bool cpu_has_ecx_feature(uint32_t feature);
bool cpu_has_ext_feature(uint64_t feature);
