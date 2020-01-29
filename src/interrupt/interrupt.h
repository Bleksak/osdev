#pragma once

#include <stdint.h>

enum INTERRUPT_DISPATCHER {
    DISPATCHER_PIC,
    DISPATCHER_IOAPIC,
    DISPATCHER_COUNT,
};

typedef void (*int_fn_install)();
typedef void (*int_fn_enable)();
typedef void (*int_fn_disable)();
typedef void (*int_fn_mask)(uint8_t irq);
typedef void (*int_fn_unmask)(uint8_t irq);
typedef void (*int_fn_eoi)(uint8_t irq);

struct InterruptDispatcher {
    const char* name;
    enum INTERRUPT_DISPATCHER id;
    int_fn_install install;
    int_fn_eoi eoi;
    int_fn_enable enable;
    int_fn_disable disable;
    int_fn_mask mask;
    int_fn_unmask unmask;
};

struct Interrupts {
    uint32_t irq_vector[256];
    const struct InterruptDispatcher* dispatcher;
};

const struct InterruptDispatcher* interrupt_find_dispatcher(const char* name);
const struct InterruptDispatcher* interrupt_find_dispatcher_id(enum INTERRUPT_DISPATCHER id);
