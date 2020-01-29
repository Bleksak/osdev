#pragma once
#include <stdint.h>

struct InterruptFrame {
    uint32_t ip, cs, flags, sp, ss;
} __attribute__((packed));

struct IDTEntry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct IDT {
    uint16_t length;
    uintptr_t idt_ptr;
} __attribute__((packed));


#define IDT_EXTRACT_PTR(entry) (uintptr_t)&(entry)

typedef struct IDT IDT;
typedef struct IDTEntry IDTEntry;

void idt_install(void);

struct ISRRegisters {
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32_t code;
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void interrupt_set_base(uint32_t index, uintptr_t base);
void interrupt_set_gate(uint32_t index, uintptr_t base, uint16_t selector, uint8_t flags);

#ifdef __clang__
    #define INTERRUPT_HANDLER(handle, fn) __attribute__((interrupt)) void handle(struct ISRRegisters* regs) {\
        fn\
    }
#else
    #define INTERRUPT_HANDLER(handle, fn)\
    __attribute__((naked)) void handle(struct ISRRegisters* regs);\
    __attribute__((unused)) static void gcc_##handle(struct ISRRegisters* regs) {fn}\
    __attribute__((naked)) void handle(struct ISRRegisters* regs) {\
            __asm__ volatile("cli");\
            __asm__ volatile("pusha");\
            __asm__ volatile("push %0" :: "r"(regs));\
            __asm__ volatile("call gcc_"#handle);\
            __asm__ volatile("add $4, %esp");\
            __asm__ volatile("popa");\
            __asm__ volatile("iretl");\
    }
#endif
