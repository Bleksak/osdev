#include "cr.h"

#include "../assert.h"

enum MSR_ADDRESSES
{
    MSR_EFER = 0xC0000080,
    MSR_XSS  = 0x0DA0,
};

static unsigned int cr0_read(void)
{
    unsigned int eax;
    __asm__ volatile("movl %%cr0, %0"
            : "=a"(eax));
    return eax;
}

unsigned int cr0_get(unsigned int mask)
{
    return cr0_read() & mask;
}

void cr0_set(unsigned int mask)
{
    unsigned int value = cr0_read() | mask;
    __asm__ volatile("mov %0, %%cr0" :: "r"(value));
}

void cr0_clear(unsigned int mask)
{
    unsigned int value = cr0_read() & ~mask;
    __asm__ volatile("mov %0, %%cr0" :: "r"(value));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

unsigned int cr3_get(void)
{
    unsigned int eax;
    __asm__ volatile("mov %0, %%cr3" :: "a"(&eax));
    return eax;
}

#pragma GCC diagnostic pop

static unsigned int cr4_read(void)
{
    unsigned int eax;
    __asm__ volatile("movl %%cr4, %0"
        : "=a"(eax));
    return eax;
}

unsigned int cr4_get(unsigned int mask)
{
    return cr4_read() & mask;
}

void cr4_clear(unsigned int mask)
{
    unsigned int value = cr4_read() & ~mask;
    __asm__ volatile("mov %0, %%cr4" :: "r"(value));
}

void cr4_set(unsigned int mask)
{
    unsigned int value = cr4_read() | mask;
    __asm__ volatile("movl %0, %%cr4" :: "r"(value));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

unsigned long long rdmsr(unsigned int ecx)
{
    unsigned int eax, edx;
    __asm__ volatile("rdmsr": "=a"(eax), "=d"(edx) : "c"(ecx));

    return (unsigned long long)edx << 32 | eax;
}

#pragma GCC diagnostic pop

void wrmsr(unsigned int ecx, unsigned long long value)
{
    // value = edx:eax

    unsigned int eax = value & 0xFFFFFFFF;
    unsigned int edx = value >> 32;

    __asm__ volatile("wrmsr" :: "c"(ecx), "a"(eax), "d"(edx));
}

static unsigned long long efer_read(void)
{
    return rdmsr(MSR_EFER);
}

unsigned long long efer_get(unsigned long long mask)
{
    return efer_read() & mask;
}

void efer_set(unsigned long long mask)
{
    wrmsr(MSR_EFER, efer_read() | mask);
}

void efer_clear(unsigned long long mask)
{
    wrmsr(MSR_EFER, efer_read() & ~mask);
}

static unsigned long long xss_read(void)
{
    return rdmsr(MSR_XSS);
}

unsigned long long xss_get(unsigned long long mask)
{
    return xss_read() & mask;
}

void xss_set(unsigned long long mask)
{
    wrmsr(MSR_XSS, xss_read() | mask);
}

void xss_clear(unsigned long long mask)
{
    wrmsr(MSR_XSS, xss_read() & ~mask);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

static unsigned long long xcr_read(unsigned int ecx)
{
    assert(ecx == 0);
    unsigned int eax, edx;

    __asm__ volatile("xgetbv": "=a"(eax), "=d"(edx) : "c"(ecx));
    return (unsigned long long)edx << 32 | eax;
}

#pragma GCC diagnostic pop

static void xcr_write(unsigned int ecx, unsigned long long mask)
{
    assert(ecx == 0);

    unsigned long long value = xcr_read(ecx) | mask;

    unsigned int eax = value & 0xFFFFFFFF;
    unsigned int edx = value >> 32;

    __asm__ volatile("xsetbv" :: "a"(eax), "c"(ecx), "d"(edx));
}

static void xcr_clear(unsigned int ecx, unsigned long long mask)
{
    assert(ecx == 0);

    unsigned long long value = xcr_read(ecx) & ~mask;

    unsigned int eax = value & 0xFFFFFFFF;
    unsigned int edx = value >> 32;

    __asm__ volatile("xsetbv" :: "a"(eax), "c"(ecx), "d"(edx));
}

static unsigned long long xcr0_read(void)
{
    return xcr_read(0);
}

unsigned long long xcr0_get(unsigned long long mask)
{
    return xcr0_read() & mask;
}

void xcr0_set(unsigned long long mask)
{
    return xcr_write(0, mask);
}

void xcr0_clear(unsigned long long mask)
{
    return xcr_clear(0, mask);
}