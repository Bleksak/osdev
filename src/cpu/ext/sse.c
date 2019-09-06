#include "sse.h"
#include "../cpuid.h"
#include "../cr.h"
#include "../../console.h"

void sse_enable(void)
{
    printf("Enabling SSE: ");
    if(!cpu_has_feature(CPUID_FEAT_SSE))
    {
        printf_colored(VGA_COLOR_RED, VGA_COLOR_BLACK, "Failed\n");
        return;
    }

    cr0_set(CR0_EM); // enable emulation
    cr4_set(CR4_OXFXSR | CR4_OSXMMEXCPT); // enable sse -> fxsave + fxrstor | exceptions

    printf_colored(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "Success\n");

    printf("POPCNT support: ");
    if(cpu_has_feature(CPUID_FEAT_POPCNT)) {
        printf_colored(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "True\n");
    }
    
    else {
        printf_colored(VGA_COLOR_RED, VGA_COLOR_BLACK, "False\n");
    }
}

void fxsave(void* buffer) {
    __asm__ volatile("fxsave %0" :: "m"(buffer));
}

void fxrstor(void* buffer) {
    __asm__ volatile("fxrstor %0" :: "m"(buffer));
}

const uint64_t m1  = 0x5555555555555555; //binary: 0101...
const uint64_t m2  = 0x3333333333333333; //binary: 00110011..
const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
const uint64_t h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,2,3...

static uint8_t popcntll_software(uint64_t x)
{
    x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
    x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits 
    x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits 
    return (x * h01) >> 56;         //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ... 
}

uint16_t popcntw(uint16_t x) {
    if(!cpu_has_feature(CPUID_FEAT_POPCNT)) {
        return (uint16_t) popcntll_software(x);
    }

    uint16_t result;
    __asm__ volatile("popcnt %1, %0" : "=r"(result) : "r"(x));
    return result;
}

uint32_t popcntl(uint32_t x) {
    if(!cpu_has_feature(CPUID_FEAT_POPCNT)) {
        return (uint32_t) popcntll_software(x);
    }

    uint32_t result;
    __asm__ volatile("popcnt %1, %0" : "=r"(result) : "r"(x));
    return result;
}

uint64_t popcntll(uint64_t x) {
    if(!cpu_has_feature(CPUID_FEAT_POPCNT)) {
        return popcntll_software(x);
    }

    #ifdef _x86
        return popcntl(x & 0xFFFFFFFF) + popcntl(x >> 32);
    #else
        uint64_t result;
        __asm__ volatile("popcnt %1, %0" : "=r"(result) : "r"(x));
        return result;
    #endif
}
