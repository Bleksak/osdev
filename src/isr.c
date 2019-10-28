#include "isr.h"
#include "interrupt.h"
// #include "pit.h"
#include "console.h"
// #include "keyboard.h"

#define PIC_MASTER_CMD 0x20
#define PIC_SLAVE_CMD  0xA0
#define PIC_MASTER_DATA PIC_MASTER_CMD + 1
#define PIC_SLAVE_DATA PIC_SLAVE_CMD + 1
#define PIC_CMD_IRR 0x0A
#define PIC_CMD_ISR 0x0B

// extern void irq0();
// extern void irq1();
// extern void irq2();
// extern void irq3();
// extern void irq4();
// extern void irq5();
// extern void irq6();
// extern void irq7();
// extern void irq8();
// extern void irq9();
// extern void irq10();
// extern void irq11();
// extern void irq12();
// extern void irq13();
// extern void irq14();
// extern void irq15();

void pic_enable(void) {
    outb(0x21, 0x0); // enable all IRQs on PICM
    outb(0xA1, 0x0); // enable all IRQs on PICS
}

void pic_disable(void) {
    outb(0xA1, 0xFF); // disable all IRQs on PICS
    outb(0x21, 0xFF); // disable all IRQs on PICM
}

void irq_install(void) {
    outb(0x20, 0x11); // write ICW1 to PICM
    outb(0xA0, 0x11); // write ICW1 to PICS

    outb(0x21, 0x20); // remap PICM to 0x20
    outb(0xA1, 0x28); // remap PICS to 0x28
    
    outb(0x21, 0x04); // IRQ2 -> IRQ8->IRQ15
    outb(0xA1, 0x02);
    
    outb(0x21, 0x01); // write ICW4 to PICM
    outb(0xA1, 0x01); // write ICW4 to PICS

    pic_disable();

    
    // interrupt_set_base(32, (uintptr_t) irq0);
    // interrupt_set_base(33, (uintptr_t) irq1);
    // interrupt_set_base(34, (uintptr_t) irq2);
    // interrupt_set_base(35, (uintptr_t) irq3);
    // interrupt_set_base(36, (uintptr_t) irq4);
    // interrupt_set_base(37, (uintptr_t) irq5);
    // interrupt_set_base(38, (uintptr_t) irq6);
    // interrupt_set_base(39, (uintptr_t) irq7);
    // interrupt_set_base(40, (uintptr_t) irq8);
    // interrupt_set_base(41, (uintptr_t) irq9);
    // interrupt_set_base(42, (uintptr_t) irq10);
    // interrupt_set_base(43, (uintptr_t) irq11);
    // interrupt_set_base(44, (uintptr_t) irq12);
    // interrupt_set_base(45, (uintptr_t) irq13);
    // interrupt_set_base(46, (uintptr_t) irq14);
    // interrupt_set_base(47, (uintptr_t) irq15);

    // pic_enable();

    // timer_install();
    // keyboard_install();
}

// static interrupt_handler_t irq_handlers[16] = {0};

// void irq_install_handler(unsigned int index, interrupt_handler_t handle) {
//     irq_handlers[index] = handle;
// }

// void irq_remove_handler(unsigned int index) {
//     irq_handlers[index] = 0;
// }

// void irq_handler(struct ISRRegisters* regs) {
//     interrupt_handler_t handle = irq_handlers[regs->code - 32];

//     if(handle) {
//         handle(regs);
//     }

//     if(regs->code >= 40) {
//         outb(0xA0, 0x20);
//     }

//     outb(0x20, 0x20);
// }
