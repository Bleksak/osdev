#pragma once

#include <stdint.h>

struct InterruptFrame {
    uint32_t ip, cs, flags;
} PACKED;

typedef unsigned long long IDTEntry;

struct IDT {
    uint16_t length;
    IDTEntry* idt_ptr;
} PACKED;

#define MAKE_IDT_ENTRY(selector, offset, type) (uintptr_t) type | (uint32_t) offset
void interrupt_handler();