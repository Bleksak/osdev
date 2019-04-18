#include "ioapic.h"

void ioapic_write(unsigned int base, unsigned char offset, unsigned int value)
{
    *(volatile unsigned int*)(base) = offset;
    *(volatile unsigned int*)(base + 0x10) = value;
}

unsigned int ioapic_read(unsigned int base, unsigned char offset)
{
    *(volatile unsigned int*)(base) = offset;
    
    return *(volatile unsigned int*)(base + 0x10);
}