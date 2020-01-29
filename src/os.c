#include "os.h"
#include <console.h>

struct OS os = {0};

void os_init(void) {
	cpu_init(&os.cpuid);
    os.interrupt.dispatcher = interrupt_find_dispatcher_id(DISPATCHER_PIC);
}
