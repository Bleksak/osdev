#include "8259.h"
#include <io.h>

#define PICM 0x20
#define PICS 0xA0

#define PICM_CMD (PICM)
#define PICS_CMD (PICS)

#define PICM_DATA (PICM + 1)
#define PICS_DATA (PICS + 1)

#define ICW1_ICW4       (1 << 0)
#define ICW1_SINGLE     (1 << 1)
#define ICW1_INTERVAL4  (1 << 2)
#define ICW1_LEVEL      (1 << 3)
#define ICW1_INIT       (1 << 4)

#define ICW4_8086       (1 << 0)
#define ICW4_AUTO       (1 << 1)
#define ICW4_BUF_SLAVE  (1 << 3)
#define ICW4_BUF_MASTER (0x0C)
#define ICW4_SFNM       (1 << 4)

#define PIC_EOI 0x20

static void pic_remap(uint8_t offset1, uint8_t offset2) {
    outb(PICM_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PICS_CMD, ICW1_INIT | ICW1_ICW4);

    outb(PICM_DATA, offset1);
    outb(PICS_DATA, offset2);

    outb(PICM_DATA, 4);
    outb(PICS_DATA, 2);

    outb(PICM_DATA, ICW4_8086);
    outb(PICS_DATA, ICW4_8086);
}

void pic_install(void) {
    pic_remap(0x20, 0x28);
    pic_enable();
}

void pic_enable(void) {
    outb(PICM_DATA, 0); // unmask every irq
    outb(PICS_DATA, 0); // unmask every irq

    __asm__ volatile("sti");
}

void pic_disable(void) {
    outb(PICM_DATA, 0xff);
    outb(PICM_DATA, 0xff);
}

void pic_eoi(uint8_t irq) {
    if(irq > 7) {
        outb(PICS_CMD, PIC_EOI);
    }

    outb(PICM_CMD, PIC_EOI);
}

void pic_mask(uint8_t irq) {
    uint16_t port = PICM_DATA;

    if(irq > 8) {
        port = PICS_DATA;
        irq -= 8;
    }

    const uint8_t save = inb(port);
    const uint8_t value = save | (1 << irq);

    outb(port, value);
}

void pic_unmask(uint8_t irq) {
    uint16_t port = PICM_DATA;

    if(irq > 8) {
        port = PICS_DATA;
        irq -= 8;
    }

    const uint8_t save = inb(port);
    const uint8_t value = save & ~(1 << irq);

    outb(port, value);
}
