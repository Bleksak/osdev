#include "io.h"

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile( "outb %b0, %1" : :"a"(value), "Nd"(port) );
}

void outw(uint16_t port, uint16_t value) {
    __asm__ volatile( "outw %w0, %1" : :"a"(value), "Nd"(port) );
}

void outl(uint16_t port, uint32_t value) {
    __asm__ volatile( "outl %0, %1" : :"a"(value), "Nd"(port) );
}

uint8_t inb(uint16_t port) {
    uint8_t res;
    __asm__ volatile( "inb %1, %b0 ": "=a"(res) : "Nd"(port) );
    return res;
}

uint16_t inw(uint16_t port) {
    uint16_t res;
    __asm__ volatile( "inw %1, %w0" : "=a"(res) : "Nd"(port) );
    return res;
}

uint32_t inl(uint16_t port) {
    uint32_t res;
    __asm__ volatile( "inl %1, %0" : "=a"(res) : "Nd"(port) );
    return res;
}

void MmioWrite32(volatile void *p, volatile uint32_t data) {
    *(volatile uint32_t *)(p) = data;
}

uint32_t MmioRead32(volatile void *p) {
    return *(volatile uint32_t *)(p);
}
