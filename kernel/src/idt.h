#pragma once

void idt_install(void);
void idt_set_gate(unsigned int num, unsigned int base, unsigned short sel, unsigned char flags);