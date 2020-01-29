#pragma once

#include <stdint.h>

void pic_install(void);
void pic_enable(void);
void pic_disable(void);

void pic_mask(uint8_t irq);
void pic_unmask(uint8_t irq);

void pic_eoi(uint8_t irq);
