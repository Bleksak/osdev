#pragma once
#include "panic.h"
#include "console.h"

#if defined(NDEBUG)
    #define assert(expr) ((void)0) 
#else
    #define assert(expr) ((expr)) ? ((void) 0) : (console_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK), printf("Assertion failed, file: %s\nin function %s\non line %u\n%s\n", __FILE__, __func__, __LINE__, #expr)); for(;;)
#endif // NDEBUG
