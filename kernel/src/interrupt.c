#include "interrupt.h"

#include "console.h"

void interrupt_handler() {
    printf("interrupt!!   ");
}

uint32_t idt_entries[1] = {
    (uint64_t)(uint32_t)interrupt_handler,
};