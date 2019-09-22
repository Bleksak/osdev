#include "io.h"

void outb(unsigned short port, unsigned char value) {
    __asm__ volatile( "outb %b0, %1" : :"a"(value), "Nd"(port) );
}

void outw(unsigned short port, unsigned short value) {
    __asm__ volatile( "outw %w0, %1" : :"a"(value), "Nd"(port) );
}

void outl(unsigned short port, unsigned int value) {
    __asm__ volatile( "outl %0, %1" : :"a"(value), "Nd"(port) );
}

unsigned char inb(unsigned short port) {
    unsigned char res;
    __asm__ volatile( "inb %1, %b0 ": "=a"(res) : "Nd"(port) );
    return res;
}

unsigned short inw(unsigned short port) {
    unsigned short res;
    __asm__ volatile( "inw %1, %w0" : "=a"(res) : "Nd"(port) );
    return res;
}

unsigned int inl(unsigned short port) {
    unsigned int res;
    __asm__ volatile( "inl %1, %0" : "=a"(res) : "Nd"(port) );
    return res;
}

void MmioWrite32(void *p, uint32_t data) {
    *(volatile uint32_t *)(p) = data;
}

uint32_t MmioRead32(void *p) {
    return *(volatile uint32_t *)(p);
}
