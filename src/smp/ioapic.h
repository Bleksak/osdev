#pragma once
#include "../std.h"

// Different pin polarities
#define IOAPIC_PIN_HIGH 0
#define IOAPIC_PIN_LOW  1

// Different trigger modes
#define IOAPIC_TRIGGER_EDGE  0
#define IOAPIC_TRIGGER_LEVEL 1

void ioapic_setup(void);
void ioapic_enable_irq(uint32_t bus, uint32_t irq, uint8_t vector, bool trigger_mode, bool polarity);
