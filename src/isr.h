#pragma once

#include "io.h"
#include "interrupt.h"

typedef void (*interrupt_handler_t) (struct ISRRegisters* regs);

void irq_install(void);
void irq_install_handler(unsigned int index, interrupt_handler_t handle);

void pic_enable(void);
void pic_disable(void);
