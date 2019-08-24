#include "cpuid.h"
#include "../console.h"
#include "../memory.h"
#include "../panic.h"

#include "cr.h"

#include "ext/fpu.h"
#include "ext/sse.h"
#include "ext/xsave.h"
#include "ext/avx.h"

struct registers
{
    unsigned int eax, ebx, ecx, edx;
};

struct CPUID cpu = 
{
    .max_cpuid = 1,
};

extern bool cpuid_available(void);
extern void enable_sse(void);

static struct registers cpuid(unsigned int eax)
{
    unsigned int ebx, ecx, edx;
    __asm__ volatile("cpuid" : "=b"(ebx), "=c"(ecx), "=d"(edx), "+a"(eax));

    return (struct registers)
    {
        .eax = eax,
        .ebx = ebx,
        .ecx = ecx,
        .edx = edx
    };
}

struct CPUID* get_cpu_info(void) {
    return &cpu;
}

bool cpu_has_feature(enum CPUID_FEATURES feature) {
    return (cpu.features & feature) == feature;
}

bool cpu_has_ext_feature(enum CPUID_EXT_FEATURES feature) {
    return (cpu.ext_features[feature / 32] & (feature % 32)) == (feature % 32);
}

void cpu_init(void)
{
    if(!cpuid_available())
    {
        panic("Error, CPUID unavailable, cannot run OS.");
    }

    struct registers vendor = cpuid(0);
    cpu.max_cpuid = vendor.eax;
    *(unsigned int*)(cpu.cpu_brand_string) = vendor.ebx;
    *(unsigned int*)(cpu.cpu_brand_string + 4) = vendor.edx;
    *(unsigned int*)(cpu.cpu_brand_string + 8) = vendor.ecx;
    
    struct registers features = cpuid(1);
    cpu.cpu_signature = features.eax;
    
    cpu.brand_index = features.ebx & 0xFF;
    cpu.clflush_line_size = (features.ebx >> 8) & 0xFF;
    cpu.logical_cpu_count = (features.ebx >> 16) & 0xFF;
    cpu.local_apic_id = (features.ebx >> 24) & 0xFF;

    cpu.features = features.edx;
    cpu.features |= ((unsigned long long) features.ecx) << 32;

    fpu_enable();
    sse_enable();
    
    xsave_enable();

    avx_enable();
    avx_512_enable();
}

