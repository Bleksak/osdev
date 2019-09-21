#include "memory.h"
#include "cpu/cpuid.h"

#include "console.h"

#define SSE_XMM_SIZE 16

static size_t min(size_t a, size_t b) {
    return (a > b) ? b : a;
}

static size_t sse_alignment(uintptr_t address) {
    return address & 15;
}

static void memcpy_sse2(void* restrict dest, void* restrict src, size_t len) {
    uintptr_t dst_addr = (uintptr_t) dest;
    uintptr_t src_addr = (uintptr_t) src;

    size_t i = min(sse_alignment(dst_addr), len);
    
    __asm__ volatile("rep movsb" : "+D"(dst_addr), "+S"(src_addr) : "c"(i));
    __asm__ volatile("prefetchnta %0" :: "m"(src_addr));
    __asm__ volatile("prefetchnta %0" :: "m"(dst_addr));

    for(; (i + 16) <= len; i += 16) {
        __asm__ volatile(
            "movdqu 0(%0), %%xmm0\n"
            "\tmovdqa %%xmm0, 0(%1)"
            :: "r"(src_addr), "r"(dst_addr) : "memory"
        );

        src_addr += 16;
        dst_addr += 16;

    }

    __asm__ volatile("rep movsb" :: "D"(dst_addr), "S"(src_addr), "c"(len - i));
}

static void memset_classic(void* restrict dest, int c, size_t len) {
    uintptr_t dst_addr = (uintptr_t) dest;
    uint32_t filled = ((uint32_t)c) * 0x0101010101;

    __asm__ volatile("rep stosl" :: "D"(dst_addr), "a"(filled), "c"(len >> 2));

    if(len & 2) {
        __asm__ volatile("stosw" :: "D"(dst_addr), "a"(filled));
    }

    if(len & 1) {
        __asm__ volatile("stosb" :: "D"(dst_addr), "a"(filled));
    }
}

static void memclr_sse2(const void * restrict const dest, const size_t len) {
    uintptr_t address = (uintptr_t) dest;

    size_t i = min(sse_alignment(address), len);

    __asm__ volatile ("rep stosb" : "+D" (address) : "c" (i), "a"(0)); // align
    __asm__ volatile("pxor %xmm0, %xmm0");
    __asm__ volatile("prefetchnta %0" :: "m"(address));

    for(; (i + 64) <= len; address += 64) {
        __asm__ volatile("movdqa %%xmm0, 0(%0)\n"
                     "\tmovdqa %%xmm0, 16(%0)\n"
                     "\tmovdqa %%xmm0, 32(%0)\n"
                     "\tmovdqa %%xmm0, 48(%0)\n"
                     :: "r"(address));
    }

    return memset_classic((void*)address, 0, len - i);
}

void memset(void* restrict dest, int c, size_t len) {
    return (cpu_has_feature(CPUID_FEAT_SSE) && c == 0) 
        ? memclr_sse2(dest, len) 
        : memset_classic(dest, c, len);
}

static void memcpy_classic(void* restrict dest, void* restrict src, size_t len) {
    uintptr_t dst_addr = (uintptr_t) dest;
    uintptr_t src_addr = (uintptr_t) src;

    __asm__ volatile("rep movsl" :: "D"(dst_addr), "S"(src_addr), "c"(len >> 2));

    if(len & 2) {
        __asm__ volatile("movsw" :: "D"(dst_addr), "S"(src_addr));
    }

    if(len & 1) {
        __asm__ volatile("movsb" :: "D"(dst_addr), "S"(src_addr));
    }
}

void memcpy(void* restrict dest, void* restrict src, size_t len) {
    if(!len)
        return;

    if(cpu_has_feature(CPUID_FEAT_SSE)) {
        return memcpy_sse2(dest, src, len);
    }

    return memcpy_classic(dest, src, len);
}

bool memcmp(void* restrict dest, void* restrict src, size_t len) {
    uint8_t* _dest = (uint8_t*) dest;
    uint8_t* _src = (uint8_t*) src;

    for(size_t i = 0; i<len; ++i)
    {
        if(_dest[i] != _src[i])
            return false;
    }

    return true;
}
