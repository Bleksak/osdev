#pragma once
#include "std.h"

void outb(unsigned short port, unsigned char value);
void outw(unsigned short port, unsigned short value);
void outl(unsigned short port, unsigned int value);

unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
unsigned int inl(unsigned short port);

void MmioWrite32(void *p, uint32_t data);

uint32_t MmioRead32(void *p);
