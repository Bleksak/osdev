#include "gdt.h"

#include "attributes.h"

#define GDT_COUNT 3

// TODO: GENERATE GDT AT COMPILE TIME

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} PACKED;

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} PACKED;

struct gdt_entry gdt[GDT_COUNT];
struct gdt_ptr gp;

extern void gdt_flush(void);

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

void gdt_install(void)
{
    gp.limit = sizeof(gdt) - 1;
    gp.base = (unsigned int)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // kernel code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // kernel data
    //gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFE, 0xCF); // user code
    //gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user data

    //tss.esp0 = getESP();

    gdt_flush();
}