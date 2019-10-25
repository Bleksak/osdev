#include "console.h"
#include "string.h"
#include "std.h"

#include "gfx/vga.h"

static volatile short* vga_buffer = (volatile short*) 0xB8000;

static uint32_t current_x = 0;
static uint32_t current_y = 0;

static uint8_t current_color = 0xF;

static void newline(void) {
    current_x = 0;
    current_y++;
}

void update_cursor(void) {
    vga_cursor_move(current_x, current_y);
}

int putch(char ch) {

    if(ch == '\n') {
        int ret = vga_width() - current_x;
        newline();
        return ret;
    }

    if(ch == '\t') {
        return 4;
    }

    vga_buffer[current_y * vga_width() + current_x] = ch | current_color << 8;
    current_x++;

    if(current_x > vga_width()) {
        newline();
    }

    return 1;
}

int puts(const char* str) {
    
    int printed_characters = 0;
    
    for(size_t i = 0; str[i]; ++i) {
        printed_characters += putch(str[i]);
    }

    return printed_characters;
}

static int do_printf(const char* format, va_list args) {
    int printed_characters = 0;

    for(size_t i = 0; format[i]; ++i) {
        if(format[i] != '%' || !format[i + 1]) {
            printed_characters += putch(format[i]);
            continue;
        }

        switch(format[i+1]) {
            case 'c': {
                printed_characters += putch(va_arg(args, int));
                i++;
            } break;

            case 's': {
                printed_characters += puts(va_arg(args, const char*));
                i++;
            } break;
            
            case 'b': {
                char buffer[33] = {0};
                itoa(va_arg(args, int), 2, buffer);
                printed_characters += puts("0b");
                printed_characters += puts(buffer);
                i++;
            } break;

            case 'p':
            case 'x': {
                char buffer[10] = {0};
                itoa(va_arg(args, int), 16, buffer);
                printed_characters += puts("0x");
                printed_characters += puts(buffer);
                i++;
            } break;

            case 'd':
            case 'u': {
                char buffer[32] = {0};
                itoa(va_arg(args, int), 10, buffer);
                printed_characters += puts(buffer);
                i++;
            } break;

            case 'l': {
                if(!format[i+2]) {
                    printed_characters += putch(format[i]);
                    continue;
                }
                
                switch(format[i+2]) {
                    case 'b': {
                        char buffer[128] = {0};
                        ltoa(va_arg(args, long long), 2, buffer);
                        printed_characters += puts("0b");

 
                        printed_characters += puts(buffer);
                        i+=2;
                    } break;

                    case 'p':
                    case 'x': {
                        char buffer[24] = {0};
                        ltoa(va_arg(args, long long), 16, buffer);
                        printed_characters += puts("0x");
                        printed_characters += puts(buffer);
                        i+=2;
                    } break;

                    case 'd':
                    case 'u': {
                        char buffer[32] = {0};
                        ltoa(va_arg(args, long long), 10, buffer);
                        printed_characters += puts(buffer);
                        i+=2;
                    } break;
                }
            } break;
        }
    }

    update_cursor();
    return printed_characters;
}
static void console_shift_left(void) {
    for(size_t i = 0; i < vga_width() - current_x - 1; ++i) {
        vga_buffer[current_y * vga_width() + current_x + i] = vga_buffer[current_y * vga_width() + current_x + i + 1];
    }
}

// static void console_shift_right(void) {
    
// }

void console_erase_back(void) {
    if(current_x == 0) {
        return;
    }

    vga_buffer[current_y * vga_width() + (--current_x)] = 0x20 | current_color << 8;

    console_shift_left();

    update_cursor();
}


void console_back(void) {
    if(!current_x) {
        return;
    }

    current_x--;
    update_cursor();
}

void console_erase_front(void) {
    const size_t y_index = current_y * vga_width();
    
    // for(size_t i = current_x; i < vga_width(); ++i) {
        
    // }
}

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = do_printf(format, args);
    va_end(args);
    return ret;
}

void panic(const char* format, ...) {
    va_list args;
    va_start(args, format);
    do_printf(format, args);
    va_end(args);

    for(;;) __asm__ __volatile("hlt");
}

void console_getcolor(enum CONSOLE_COLORS* fg, enum CONSOLE_COLORS* bg) {
    *fg = current_color & 0x0F;
    *bg = (current_color >> 4) & 0x0F;
}

void console_setcolor(enum CONSOLE_COLORS text, enum CONSOLE_COLORS bg) {
    current_color = text | bg << 4;
}

int printf_colored(enum CONSOLE_COLORS fg, enum CONSOLE_COLORS bg, const char* format, ...) {
    
    enum CONSOLE_COLORS backup_fg, backup_bg;
    console_getcolor(&backup_fg, &backup_bg);
    
    console_setcolor(fg, bg);

    va_list args;
    va_start(args, format);
    
    int ret = do_printf(format, args);

    va_end(args);

    console_setcolor(backup_fg, backup_bg);
    return ret;
}
