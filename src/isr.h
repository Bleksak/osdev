#pragma once

#include "io.h"

struct registers {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int interrupt, error;
    unsigned int eip, cs, eflags, user_esp, ss;
};

typedef void (*interrupt_handler_t) (struct registers* regs);

void irq_install(void);
void irq_install_handler(unsigned int index, interrupt_handler_t handle);

void pic_enable(void);
void pic_disable(void);
