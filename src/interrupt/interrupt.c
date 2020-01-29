#include <stddef.h>
#include <str.h>
#include "interrupt.h"
#include "8259.h"

const static struct InterruptDispatcher pic_dispatcher = {
    .name = "PIC",
    .id = DISPATCHER_PIC,
    .install = pic_install,
    .eoi = pic_eoi,
    .enable = pic_enable,
    .disable = pic_disable,
    .mask = pic_mask,
    .unmask = pic_unmask,
};

const static struct InterruptDispatcher ioapic_dispatcher = {
    .name = "IOAPIC",
    .id = DISPATCHER_IOAPIC,
};

const static struct InterruptDispatcher* interrupt_dispatchers[] = {
    &pic_dispatcher,
    &ioapic_dispatcher,
};


const struct InterruptDispatcher* interrupt_find_dispatcher_id(enum INTERRUPT_DISPATCHER id) {
    for(size_t i = 0; i < sizeof(interrupt_dispatchers)/sizeof(interrupt_dispatchers[0]); ++i) {
        if(interrupt_dispatchers[i]->id == id) {
            return interrupt_dispatchers[i];
        }
    }

    return 0;
}

const struct InterruptDispatcher* interrupt_find_dispatcher(const char* name) {

    return &pic_dispatcher;

    // for(size_t i = 0; i < sizeof(interrupt_dispatchers)/sizeof(interrupt_dispatchers[0]); ++i) {
    //     if(str_cmp(name, interrupt_dispatchers[i]->name)) {
    //         return interrupt_dispatchers[i];
    //     }
    // }

    // return 0;
}
