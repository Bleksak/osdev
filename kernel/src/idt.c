#include "idt.h"
#include "isr.h"

#include "console.h"

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

struct idt_entry
{
    unsigned short base_low;
    unsigned short sel;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

extern void idt_load(unsigned int);

struct idt_pointer
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_pointer idt_ptr;

void idt_set_gate(unsigned int num, unsigned int base, unsigned short sel, unsigned char flags)
{
    idt[num].base_high = (base>>16) & 0xffff;
    idt[num].base_low = base & 0xffff;
    idt[num].sel = sel;
    idt[num].flags = flags;
    idt[num].zero = 0;
}

void idt_install()
{
    idt_ptr.limit = (sizeof(struct idt_entry) * 256 - 1);
    idt_ptr.base = (unsigned int)&idt;

    idt_set_gate(0,  (unsigned int) isr0,  8, 0x8E);
    idt_set_gate(1,  (unsigned int) isr1,  8, 0x8E);
    idt_set_gate(2,  (unsigned int) isr2,  8, 0x8E);
    idt_set_gate(3,  (unsigned int) isr3,  8, 0x8E);
    idt_set_gate(4,  (unsigned int) isr4,  8, 0x8E);
    idt_set_gate(5,  (unsigned int) isr5,  8, 0x8E);
    idt_set_gate(6,  (unsigned int) isr6,  8, 0x8E);
    idt_set_gate(7,  (unsigned int) isr7,  8, 0x8E);
    idt_set_gate(8,  (unsigned int) isr8,  8, 0x8E);
    idt_set_gate(9,  (unsigned int) isr9,  8, 0x8E);
    idt_set_gate(10, (unsigned int) isr10, 8, 0x8E);
    idt_set_gate(11, (unsigned int) isr11, 8, 0x8E);
    idt_set_gate(12, (unsigned int) isr12, 8, 0x8E);
    idt_set_gate(13, (unsigned int) isr13, 8, 0x8E);
    idt_set_gate(14, (unsigned int) isr14, 8, 0x8E);
    idt_set_gate(15, (unsigned int) isr15, 8, 0x8E);
    idt_set_gate(16, (unsigned int) isr16, 8, 0x8E);
    idt_set_gate(17, (unsigned int) isr17, 8, 0x8E);
    idt_set_gate(18, (unsigned int) isr18, 8, 0x8E);
    idt_set_gate(19, (unsigned int) isr19, 8, 0x8E);
    idt_set_gate(20, (unsigned int) isr20, 8, 0x8E);
    idt_set_gate(21, (unsigned int) isr21, 8, 0x8E);
    idt_set_gate(22, (unsigned int) isr22, 8, 0x8E);
    idt_set_gate(23, (unsigned int) isr23, 8, 0x8E);
    idt_set_gate(24, (unsigned int) isr24, 8, 0x8E);
    idt_set_gate(25, (unsigned int) isr25, 8, 0x8E);
    idt_set_gate(26, (unsigned int) isr26, 8, 0x8E);
    idt_set_gate(27, (unsigned int) isr27, 8, 0x8E);
    idt_set_gate(28, (unsigned int) isr28, 8, 0x8E);
    idt_set_gate(29, (unsigned int) isr29, 8, 0x8E);
    idt_set_gate(30, (unsigned int) isr30, 8, 0x8E);
    idt_set_gate(31, (unsigned int) isr31, 8, 0x8E);

    irq_install();

    idt_load((unsigned int)&idt_ptr);
}