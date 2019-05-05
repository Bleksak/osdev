#include "panic.h"
#include "console.h"

void panic(const char* str)
{
    console_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf("%s\n", str);
    
    for(;;);
}