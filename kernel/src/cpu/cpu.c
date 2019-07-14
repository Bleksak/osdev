#include "cpu/cpu.h"
#include "console.h"
#include "memory.h"
#include "panic.h"


#include "asm.h"

struct registers
{
    unsigned int eax, ebx, ecx, edx;
};

struct cpuid cpu;

static struct registers regs;

extern unsigned char cpuid_available();
extern void enable_sse();

static inline void cpuid(unsigned int eax)
{
    unsigned int ebx, ecx, edx;
    asm volatile("cpuid" : "=b"(ebx), "=c"(ecx), "=d"(edx), "+a"(eax));
    
    regs.eax = eax;
    regs.ebx = ebx;
    regs.ecx = ecx;
    regs.edx = edx;
}

struct cpuid* get_cpu_info(void)
{
    return &cpu;
}

void cpu_init(void)
{

    if(!cpuid_available())
    {
        panic("Error, CPUID unavailable, cannot run OS.");
    }


    cpuid(0);
    cpu.maximum_cpuid_input_value = regs.eax;
    *(unsigned int*)(cpu.cpu_brand_string) = regs.ebx;
    *(unsigned int*)(cpu.cpu_brand_string + 4) = regs.edx;
    *(unsigned int*)(cpu.cpu_brand_string + 8) = regs.ecx;
    
    cpuid(1);
    cpu.cpu_signature = regs.eax;
    
    cpu.brand_index = regs.ebx & 0xFF;
    cpu.clflush_line_size = (regs.ebx >> 8) & 0xFF;
    cpu.logical_cpu_count = (regs.ebx >> 16) & 0xFF;
    cpu.local_apic_id = (regs.ebx >> 24) & 0xFF;

    *(unsigned int*)(&cpu.features) = regs.edx;
    *((unsigned int*)(&cpu.features) + 1)= regs.ecx;
    
    if(!cpu.features.msr)
    {
        goto cpu_old;
    }

    if(!cpu.features.sse)
    {
        goto cpu_old;
    }

    if(!cpu.features.apic)
    {
        goto cpu_old;
    }

    enable_sse();

    return;

    cpu_old:
    panic("Your CPU is too old");
}