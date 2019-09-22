#include "pit.h"
#include "io.h"
#include "isr.h"
#include "console.h"

void timer_phase(int hz)
{
    int divisor = 1193180 / hz;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xff);
    outb(0x40, divisor >> 8);
}

volatile unsigned int ticks = 0;

void timer_handler(struct registers* regs)
{
    (void) regs;
    ++ticks;
}

void timer_wait(unsigned int seconds)
{
    volatile unsigned int end = ticks + seconds * 18;
    while(end>ticks);
}

void timer_install(void) {
    irq_install_handler(0, timer_handler);
}

// void interrupt_timer_handler(struct InterruptFrame* frame) {
//     (void) frame;
//     ++ticks;
//     printf("Tick\n");
// }
