#include "cpuid.h"
#include "../console.h"
#include "../memory.h"
#include "../os.h"

#include "cr.h"

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

struct Registers {
    unsigned int eax, ebx, ecx, edx, esi, edi, ebp, esp;
};

static Registers cpuid(uint32_t eax) {
    unsigned int ebx, ecx, edx;
    __asm__ volatile("cpuid" : "=b"(ebx), "=c"(ecx), "=d"(edx), "+a"(eax));

    return (Registers) {
        .eax = eax,
        .ebx = ebx,
        .ecx = ecx,
        .edx = edx
    };
}

bool cpu_has_ecx_feature(uint32_t feature) {
    return (os.cpuid.features.ecx & feature) == feature;
}

bool cpu_has_edx_feature(uint32_t feature) {
    return (os.cpuid.features.edx & feature) == feature;
}

bool cpu_has_ext_feature(uint64_t feature) {
    return (os.cpuid.ext_features[feature / 32] & (feature % 32)) == (feature % 32);
}

void cpu_init(void) {
    if(!cpuid_available()) {
        panic("Error, CPUID unavailable, cannot run OS.");
    }

    Registers vendor = cpuid(0);
    os.cpuid.max_cpuid = vendor.eax;
    *(unsigned int*)(os.cpuid.cpu_brand_string) = vendor.ebx;
    *(unsigned int*)(os.cpuid.cpu_brand_string + 4) = vendor.edx;
    *(unsigned int*)(os.cpuid.cpu_brand_string + 8) = vendor.ecx;
    
    Registers features = cpuid(1);
    os.cpuid.cpu_signature = features.eax;
    
    os.cpuid.brand_index = features.ebx & 0xFF;
    os.cpuid.clflush_line_size = (features.ebx >> 8) & 0xFF;
    os.cpuid.logical_cpu_count = (features.ebx >> 16) & 0xFF;
    os.cpuid.local_apic_id = (features.ebx >> 24) & 0xFF;

    os.cpuid.features.ecx = features.ecx;
    os.cpuid.features.edx = features.edx;

    fpu_enable();
    sse_enable();
    
    xsave_enable();

    avx_enable();
    avx_512_enable();
}

