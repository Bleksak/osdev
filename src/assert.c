#include "assert.h"

void __assert(const char* file, const char* function, int line, const char* expression) {
    console_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
    printf("Assertion failed, file: %s\nin function %s\non line %d\n%s\n", file, function, line, expression);
    for(;;);
}
