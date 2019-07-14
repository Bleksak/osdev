#include "io.h"

#include "asm.h"

void outb(unsigned short port, unsigned char value)
{
    asm volatile( "outb %b0, %1" : :"a"(value), "Nd"(port) );
}

void outw(unsigned short port, unsigned short value)
{
    asm volatile( "outw %w0, %1" : :"a"(value), "Nd"(port) );
}

void outl(unsigned short port, unsigned int value)
{
    asm volatile( "outl %0, %1" : :"a"(value), "Nd"(port) );
}

unsigned char inb(unsigned short port)
{
    unsigned char res;
    asm volatile( "inb %1, %b0 ": "=a"(res) : "Nd"(port) );
    return res;
}

unsigned short inw(unsigned short port)
{
    unsigned short res;
    asm volatile( "inw %1, %w0" : "=a"(res) : "Nd"(port) );
    return res;
}

unsigned int inl(unsigned short port)
{
    unsigned int res;
    asm volatile( "inl %1, %0" : "=a"(res) : "Nd"(port) );
    return res;
}

void MmioWrite32(void *p, uint32_t data)
{
    *(volatile uint32_t *)(p) = data;
}

uint32_t MmioRead32(void *p)
{
    return *(volatile uint32_t *)(p);
}