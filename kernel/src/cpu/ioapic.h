#pragma once

unsigned int ioapic_read(unsigned int base, unsigned char offset);
void ioapic_write(unsigned int base, unsigned char offset, unsigned int value);

