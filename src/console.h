#pragma once

#include "std.h"

enum CONSOLE_COLORS
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void clearscreen(void);

void update_cursor(void);

int putch(char ch);
int printf(const char* str, ...);
int printf_colored(enum CONSOLE_COLORS fg, enum CONSOLE_COLORS bg, const char* str, ...);

void console_setcolor(enum CONSOLE_COLORS text, enum CONSOLE_COLORS bg);
void console_getcolor(enum CONSOLE_COLORS* fg, enum CONSOLE_COLORS* bg);

void console_back(void);
void console_erase_back(void);

__attribute__((noreturn)) void panic(const char* str, ...);
