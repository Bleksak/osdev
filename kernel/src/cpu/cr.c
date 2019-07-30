#include "cr.h"
#include "asm.h"

static unsigned int cr0_read(void)
{
    unsigned int cr0;
    asm volatile("movl %%cr0, %0"
            : "=r"(cr0));
    return cr0;
}

unsigned int cr0_get(unsigned int mask)
{
    return cr0_read() & mask;
}

void cr0_set(unsigned int mask)
{
    unsigned int value = cr0_read() | mask;
    asm volatile("mov %0, %cr0" : "r"(value));
}