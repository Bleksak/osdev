#include "cr.h"

#include <assert.h>

static uint32_t cr0_read(void) {
    uint32_t eax;
    __asm__ volatile("movl %%cr0, %0"
            : "=a"(eax));
    return eax;
}

uint32_t cr0_get(uint32_t mask) {
    return cr0_read() & mask;
}

void cr0_set(uint32_t mask) {
    uint32_t value = cr0_read() | mask;
    __asm__ volatile("mov %0, %%cr0" :: "r"(value));
}

void cr0_clear(uint32_t mask) {
    uint32_t value = cr0_read() & ~mask;
    __asm__ volatile("mov %0, %%cr0" :: "r"(value));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

uint32_t cr3_get(void) {
    uint32_t eax;
    __asm__ volatile("mov %0, %%cr3" :: "a"(&eax));
    return eax;
}

#pragma GCC diagnostic pop

static uint32_t cr4_read(void) {
    uint32_t eax;
    __asm__ volatile("movl %%cr4, %0"
        : "=a"(eax));
    return eax;
}

uint32_t cr4_get(uint32_t mask)
{
    return cr4_read() & mask;
}

void cr4_clear(uint32_t mask) {
    uint32_t value = cr4_read() & ~mask;
    __asm__ volatile("mov %0, %%cr4" :: "r"(value));
}

void cr4_set(uint32_t mask) {
    uint32_t value = cr4_read() | mask;
    __asm__ volatile("movl %0, %%cr4" :: "r"(value));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

uint64_t rdmsr(uint32_t ecx) {
    uint32_t eax, edx;
    __asm__ volatile("rdmsr": "=a"(eax), "=d"(edx) : "c"(ecx));

    return (uint64_t)edx << 32 | eax;
}

#pragma GCC diagnostic pop

void wrmsr(uint32_t ecx, uint64_t value) {
    // value = edx:eax

    uint32_t eax = value & 0xFFFFFFFF;
    uint32_t edx = value >> 32;

    __asm__ volatile("wrmsr" :: "c"(ecx), "a"(eax), "d"(edx));
}

static uint64_t efer_read(void) {
    return rdmsr(MSR_EFER);
}

uint64_t efer_get(uint64_t mask) {
    return efer_read() & mask;
}

void efer_set(uint64_t mask) {
    wrmsr(MSR_EFER, efer_read() | mask);
}

void efer_clear(uint64_t mask) {
    wrmsr(MSR_EFER, efer_read() & ~mask);
}

static uint64_t xss_read(void) {
    return rdmsr(MSR_XSS);
}

uint64_t xss_get(uint64_t mask) {
    return xss_read() & mask;
}

void xss_set(uint64_t mask) {
    wrmsr(MSR_XSS, xss_read() | mask);
}

void xss_clear(uint64_t mask) {
    wrmsr(MSR_XSS, xss_read() & ~mask);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

static uint64_t xcr_read(uint32_t ecx) {
    assert(ecx == 0);
    uint32_t eax, edx;

    __asm__ volatile("xgetbv": "=a"(eax), "=d"(edx) : "c"(ecx));
    return (uint64_t)edx << 32 | eax;
}

#pragma GCC diagnostic pop

static void xcr_write(uint32_t ecx, uint64_t mask) {
    assert(ecx == 0);

    uint64_t value = xcr_read(ecx) | mask;

    uint32_t eax = value & 0xFFFFFFFF;
    uint32_t edx = value >> 32;

    __asm__ volatile("xsetbv" :: "a"(eax), "c"(ecx), "d"(edx));
}

static void xcr_clear(uint32_t ecx, uint64_t mask) {
    assert(ecx == 0);

    uint64_t value = xcr_read(ecx) & ~mask;

    uint32_t eax = value & 0xFFFFFFFF;
    uint32_t edx = value >> 32;

    __asm__ volatile("xsetbv" :: "a"(eax), "c"(ecx), "d"(edx));
}

static uint64_t xcr0_read(void) {
    return xcr_read(0);
}

uint64_t xcr0_get(uint64_t mask) {
    return xcr0_read() & mask;
}

void xcr0_set(uint64_t mask) {
    xcr_write(0, mask);
}

void xcr0_clear(uint64_t mask) {
    xcr_clear(0, mask);
}
