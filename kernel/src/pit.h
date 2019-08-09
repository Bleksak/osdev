#pragma once
#include "interrupt.h"

void timer_install(void);
void timer_phase(int hz);
void timer_wait(unsigned int seconds);

INTERRUPT void interrupt_timer_handler(struct InterruptFrame* frame);
