#include "isr.h"
#include "pit.h"
#include "console.h"
#include "keyboard.h"

#include "tss.h"

extern void idt_set_gate(unsigned int num, unsigned int base, unsigned short sel, unsigned char flags);

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();


static char* exception_messages[32] = 
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception",
    "Coprocessor Fault Exception",
    "Alignment Check Exception",
    "Machine Check Exception",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

enum ISR
{
    DIVISION_BY_ZERO,
    DEBUG,
    NMI,
    BREAKPOINT,
    INTO_DETECTED_OVERFLOW,
    OUT_OF_BOUNDS,
    INVALID_OPCODE,
    NO_COPROCESSOR,
    DOUBLE_FAULT,
    COPROCESSOR_SEGMENT_OVERRUN,
    BAD_TSS,
    SEGMENT_NOT_PRESENT,
    STACK_FAULT,
    GENERAL_PROTECTION_FAULT,
    PAGE_FAULT,
    UNKNOWN_INTERRUPT,
    COPROCESSOR_FAULT,
    ALIGNMENT_CHECK,
    MACHINE_CHECK,
};

void isr_handler(struct registers* regs)
{
    printf("%s\n", exception_messages[regs->interrupt]);
}

void pic_enable(void)
{
    outb(0x21, 0x0); // enable all IRQs on PICM
    outb(0xA1, 0x0); // enable all IRQs on PICS
}

void pic_disable(void)
{
    outb(0x21, 0xFF); // disable all IRQs on PICM
    outb(0xA1, 0xFF); // disable all IRQs on PICS
}

void irq_install(void)
{
    outb(0x20, 0x11); // write ICW1 to PICM
    outb(0xA0, 0x11); // write ICW1 to PICS

    outb(0x21, 0x20); // remap PICM to 0x20
    outb(0xA1, 0x28); // remap PICS to 0x28
    
    outb(0x21, 0x04); // IRQ2 -> IRQ8->IRQ15
    outb(0xA1, 0x02);
    
    outb(0x21, 0x01); // write ICW4 to PICM
    outb(0xA1, 0x01); // write ICW4 to PICS

    pic_enable();

    idt_set_gate(32, (unsigned int)irq0,  8, 0x8F);
    idt_set_gate(33, (unsigned int)irq1,  8, 0x8F);
    idt_set_gate(34, (unsigned int)irq2,  8, 0x8F);
    idt_set_gate(35, (unsigned int)irq3,  8, 0x8F);
    idt_set_gate(36, (unsigned int)irq4,  8, 0x8F);
    idt_set_gate(37, (unsigned int)irq5,  8, 0x8F);
    idt_set_gate(38, (unsigned int)irq6,  8, 0x8F);
    idt_set_gate(39, (unsigned int)irq7,  8, 0x8F);
    idt_set_gate(40, (unsigned int)irq8,  8, 0x8F);
    idt_set_gate(41, (unsigned int)irq9,  8, 0x8F);
    idt_set_gate(42, (unsigned int)irq10, 8, 0x8F);
    idt_set_gate(43, (unsigned int)irq11, 8, 0x8F);
    idt_set_gate(44, (unsigned int)irq12, 8, 0x8F);
    idt_set_gate(45, (unsigned int)irq13, 8, 0x8F);
    idt_set_gate(46, (unsigned int)irq14, 8, 0x8F);
    idt_set_gate(47, (unsigned int)irq15, 8, 0x8F);

    timer_install();
    keyboard_install();
}

void* irq_handlers[16] = 
{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

void irq_install_handler(unsigned int index, void* handle)
{
    irq_handlers[index] = handle;
}

void irq_remove_handler(unsigned int index)
{
    irq_handlers[index] = 0;
}

void irq_handler(struct registers* regs)
{
    void (*handle)(struct registers*) = irq_handlers[regs->interrupt - 32];

    if(regs->interrupt != 32)
        printf("Interrupt  ");

    if(handle)
    {
        handle(regs);
    }

    if(regs->interrupt >= 40)
    {
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);
}