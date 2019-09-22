#include "gdt.h"

#include "attributes.h"

// TODO: GENERATE GDT AT COMPILE TIME

struct GDTEntry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} PACKED;

struct GDT
{
    unsigned short limit;
    unsigned int base;
} PACKED;

#define MAKE_GDT_ENTRY(base, limit, access, granularity) ((limit & 0xFFFF) << 48) | ((base & 0xFFFF) << 32) | (((base >> 16) && 0xFF) << 24) | (access & 0xFF) << 16 | ((granularity & 0xF0) | (limit >> 16) & 0x0F) | 

const struct GDTEntry gdt[] = {
    {
        .base_low = 0,
        .base_middle = 0,
        .base_high = 0,
        .limit_low = 0,
        .granularity = 0,
        .access = 0,
    },
    {
        .base_low = 0,
        .base_middle = 0,
        .base_high = 0,
        .limit_low = 0xFFFF,
        .granularity = 0xCF,
        .access = 0x9A,
    },
    {
        .base_low = 0,
        .base_middle = 0,
        .base_high = 0,
        .limit_low = 0xFFFF,
        .granularity = 0xCF,
        .access = 0x92,
    }
};

/*
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}
*/

__attribute__((naked)) static void gdt_flush(void) {
    __asm__ volatile("movl %0, %%ds;"
                     "movl %0, %%es;" 
                     "movl %0, %%fs;"
                     "movl %0, %%gs;"
                     "movl %0, %%ss;"
                     "jmp $8, $.flush;"
                     ".flush:;"
                     "ret" :: "r"(0x10));
}

static void lgdt(struct GDT* gdtr) {
    __asm__ volatile("lgdt (%0)" :: "r"(gdtr));
}

void gdt_install(void)
{
    struct GDT gdtr;
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (unsigned int)&gdt;

    //gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFE, 0xCF); // user code
    //gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user data

    lgdt(&gdtr);

    gdt_flush();
}
