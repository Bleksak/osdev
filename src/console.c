#include "console.h"
#include "string.h"
#include "std.h"

unsigned int current_x = 0;
unsigned int current_y = 0;
unsigned char current_color = 0x0f;

#define screen_width 160

volatile char* vga_buffer = (volatile char*) 0xb8000;

void itoa(unsigned int num, unsigned char base, char* buffer) {
    if(num == 0) {
        buffer[0] = '0';
        buffer[1] = 0;
        return;
    }

    unsigned char i = 0;

    while(num) {
        buffer[i++] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[num % base];
        num /= base;
    }
    
    buffer[i] = 0;

    strrev(buffer);
}

void ltoa(uint64_t num, uint8_t base, char* buffer) {
    if(num == 0) {
        buffer[0] = '0';
        buffer[1] = 0;
        return;
    }

    uint8_t i = 0;

    while(num) {
        buffer[i++] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[num % base];
        num /= base;
    }

    buffer[i] = 0;
    strrev(buffer);
}


void clearscreen() {
    for(unsigned i = 0; i<32768; ++i) {
        vga_buffer[i] = 0;
    }

    current_x = 0;
    current_y = 0;
}

void putch(char ch) {
    if(ch == '\n') {
        current_x = 0;
        ++current_y;
        return;
    }
    
    vga_buffer[(current_x<<1) + current_y * screen_width] = ch;
    vga_buffer[(current_x<<1) + 1 + current_y * screen_width] = current_color;
    
    if(++current_x > screen_width) {
        ++current_y;
        current_x = 0;
    }
}

void console_setcolor(enum CONSOLE_COLORS fg, enum CONSOLE_COLORS bg) {
    current_color = fg | bg << 4;
}

static inline void puts(const char* str) {
    while(*str)
        putch(*str++);
}

static void do_printf(const char* str, va_list list) {
    for(unsigned int i = 0; i<strlen((char*)str); ++i) {
        char current_char = str[i];

        if(current_char == '%' && str[i-1] != '\\') {
            char next_char = str[i+1];
            switch(next_char) {
                case 'c': {
                    putch((char)va_arg(list, int));
                } break;

                case 's': {
                    puts((const char*)va_arg(list, const char*));
                } break;

                case 'd':
                case 'u': {
                    static char buffer[10] = {0};
                    itoa(va_arg(list, int), 10, buffer);
                    puts(buffer);
                } break;


                case 'x': {
                    static char buffer[16] = {0};
                    puts("0x");
                    itoa(va_arg(list, int), 16, buffer);
                    puts(buffer);
                } break;

                case 'p': {
                    static char buffer[16] = {0};
                    puts("0x");
                    itoa((unsigned int)va_arg(list, void*), 16, buffer);
                    puts(buffer);
                } break;

                case 'l': {
                    char next = str[i+2];
                    if(next == 'd' || next == 'u') {
                        static char buffer[24] = {0};
                        ltoa((unsigned long long)va_arg(list, uint64_t), 10, buffer);
                        puts(buffer);
                    } else if (next == 'x') {
                        static char buffer[24] = {0};
                        ltoa((unsigned long long)va_arg(list, uint64_t), 16, buffer);
                        puts("0x");
                        puts(buffer);
                    }
                    ++i;
                } break;
            }

        ++i;
        continue;
        
        }

        putch(current_char);
    }
}

void printf_colored(enum CONSOLE_COLORS fg, enum CONSOLE_COLORS bg, const char* str, ...) {
    console_setcolor(fg, bg);
    va_list list;
    va_start(list, str);
    do_printf(str, list);
    va_end(list);
    console_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

void printf(const char* str, ...) {
    va_list va_list;
    va_start(va_list, str);
    do_printf(str, va_list);
    va_end(va_list);
}
