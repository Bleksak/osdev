#pragma once
#include "apic.h"


void ioapic_setup(uintptr_t base);
void ioapic_enable_irq(uint32_t irq, uint8_t vector);