#include "interrupt.h"
#include "attributes.h"
#include "console.h"

#include "isr.h"

#include "pit.h"
#include "console.h"

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

IDTEntry idt[256] = {
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8E,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
    {
        .base_high = 0,
        .base_low = 0,
        .sel = 8,
        .flags = 0x8F,
        .zero = 0,
    },
};

void interrupt_set_base(unsigned int index, unsigned int base) {
    idt[index].base_high = (base >> 16) & 0xFFFF;
    idt[index].base_low = base & 0xFFFF;
}

// void idt_set_gate(unsigned int num, unsigned int base, unsigned short sel, unsigned char flags)
// {
//     idt[num].base_high = (base>>16) & 0xffff;
//     idt[num].base_low = base & 0xffff;
//     idt[num].sel = sel;
//     idt[num].flags = flags;
//     idt[num].zero = 0;
// }

static void lidt(const IDT* idtr) {
    __asm__ volatile("lidt (%0)" :: "r"(idtr));
}

void idt_install(void) {
    interrupt_set_base(0,  (uintptr_t)isr0);
    interrupt_set_base(1,  (uintptr_t)isr1);
    interrupt_set_base(2,  (uintptr_t)isr2);
    interrupt_set_base(3,  (uintptr_t)isr3);
    interrupt_set_base(4,  (uintptr_t)isr4);
    interrupt_set_base(5,  (uintptr_t)isr5);
    interrupt_set_base(6,  (uintptr_t)isr6);
    interrupt_set_base(7,  (uintptr_t)isr7);
    interrupt_set_base(8,  (uintptr_t)isr8);
    interrupt_set_base(9,  (uintptr_t)isr9);
    interrupt_set_base(10, (uintptr_t)isr10);
    interrupt_set_base(11, (uintptr_t)isr11);
    interrupt_set_base(12, (uintptr_t)isr12);
    interrupt_set_base(13, (uintptr_t)isr13);
    interrupt_set_base(14, (uintptr_t)isr14);
    interrupt_set_base(15, (uintptr_t)isr15);
    interrupt_set_base(16, (uintptr_t)isr16);
    interrupt_set_base(17, (uintptr_t)isr17);
    interrupt_set_base(18, (uintptr_t)isr18);
    interrupt_set_base(19, (uintptr_t)isr19);
    interrupt_set_base(20, (uintptr_t)isr20);
    interrupt_set_base(21, (uintptr_t)isr21);
    interrupt_set_base(22, (uintptr_t)isr22);
    interrupt_set_base(23, (uintptr_t)isr23);
    interrupt_set_base(24, (uintptr_t)isr24);
    interrupt_set_base(25, (uintptr_t)isr25);
    interrupt_set_base(26, (uintptr_t)isr26);
    interrupt_set_base(27, (uintptr_t)isr27);
    interrupt_set_base(28, (uintptr_t)isr28);
    interrupt_set_base(29, (uintptr_t)isr29);
    interrupt_set_base(30, (uintptr_t)isr30);
    interrupt_set_base(31, (uintptr_t)isr31);
    
    irq_install();

    const IDT idtr = {
        .idt_ptr = IDT_EXTRACT_PTR(idt),
        .length = sizeof(idt) - 1,
    };

    lidt(&idtr);
}