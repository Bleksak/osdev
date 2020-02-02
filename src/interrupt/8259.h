#pragma once

#include <stdint.h>
#include <stdbool.h>

void pic_install(void);
void pic_enable(void);
void pic_disable(void);

void pic_mask(uint32_t bus, uint32_t irq);
void pic_unmask(uint32_t bus, uint32_t irq, uint8_t vector, bool trigger_mode, bool polarity);

void pic_eoi(uint8_t irq);
