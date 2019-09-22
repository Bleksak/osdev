#pragma once
#include "panic.h"
#include "console.h"

#ifdef NDEBUG
    #define assert(expr) ((void)0) 
#else
    #define assert(expr) (void)((expr) || (__assert (__FILE__, __func__, __LINE__, #expr),0))
#endif // NDEBUG

NORETURN void __assert(const char* file, const char* function, int line, const char* expression);
