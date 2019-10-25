#pragma once

#include "std.h"

#include "attributes.h"

struct InterruptFrame {
    uint32_t ip, cs, flags, sp, ss;
} PACKED;

struct IDTEntry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} PACKED;

struct IDT {
    uint16_t length;
    uintptr_t idt_ptr;
} PACKED;


#define IDT_EXTRACT_PTR(entry) (uintptr_t)&entry

typedef struct IDT IDT;
typedef struct IDTEntry IDTEntry;

void idt_install(void);

struct ISRRegisters {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int code;
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void interrupt_set_base(unsigned int index, unsigned int base);
void interrupt_handler();
