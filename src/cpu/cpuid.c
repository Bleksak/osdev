#include "cpuid.h"
#include "../console.h"
#include "../memory.h"

#include "cr.h"

static struct CPUID cpu = {
    .max_cpuid = 1,
};

static bool cpuid_available(void) {
    bool result;
    __asm__ volatile (
        "pushfl;"                               //Save EFLAGS
        "pushfl;"                               //Store EFLAGS
        "xorl $0x00200000, (%%esp);"           //Invert the ID bit in stored EFLAGS
        "popfl;"                                //Load stored EFLAGS (with ID bit inverted)
        "pushfl;"                               //Store EFLAGS again (ID bit may or may not be inverted)
        "pop %%eax;"                              //eax = modified EFLAGS (ID bit may or may not be inverted)
        "xorl (%%esp), %%eax;"                        //eax = whichever bits were changed
        "popfl;"                                //Restore original EFLAGS
        "and $0x00200000, %%eax;"                   //eax = zero if ID bit can't be changed, else non-zero
        "shr $21, %%eax;"
        : "=a" (result)
        : 
        : "cc"
    );

    return result;
}

static Registers cpuid(unsigned int eax) {
    unsigned int ebx, ecx, edx;
    __asm__ volatile("cpuid" : "=b"(ebx), "=c"(ecx), "=d"(edx), "+a"(eax));

    return (Registers)
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

bool cpu_has_feature(uint64_t feature) {
    return (cpu.features[feature / 32] & (feature % 32)) == (feature % 32);
}

bool cpu_has_ext_feature(uint64_t feature) {
    return (cpu.ext_features[feature / 32] & (feature % 32)) == (feature % 32);
}

void cpu_init(void) {
    if(!cpuid_available()) {
        panic("Error, CPUID unavailable, cannot run OS.");
    }

    Registers vendor = cpuid(0);
    cpu.max_cpuid = vendor.eax;
    *(unsigned int*)(cpu.cpu_brand_string) = vendor.ebx;
    *(unsigned int*)(cpu.cpu_brand_string + 4) = vendor.edx;
    *(unsigned int*)(cpu.cpu_brand_string + 8) = vendor.ecx;
    
    Registers features = cpuid(1);
    cpu.cpu_signature = features.eax;
    
    cpu.brand_index = features.ebx & 0xFF;
    cpu.clflush_line_size = (features.ebx >> 8) & 0xFF;
    cpu.logical_cpu_count = (features.ebx >> 16) & 0xFF;
    cpu.local_apic_id = (features.ebx >> 24) & 0xFF;

    cpu.features[0] = features.edx;
    cpu.features[1] = features.ecx;

    fpu_enable();
    sse_enable();
    
    xsave_enable();

    avx_enable();
    avx_512_enable();
}

