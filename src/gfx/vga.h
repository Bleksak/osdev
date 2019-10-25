#pragma once

#include "../std.h"

uint16_t vga_width(void);

void vga_cursor_enable(uint8_t cursor_start, uint8_t cursor_end);
void vga_cursor_disable(void);
void vga_cursor_move(uint16_t x, uint16_t y);
void vga_cursor_change(uint16_t dx, uint16_t dy);
void vga_cursor_move_pos(uint16_t position);
void vga_cursor_change_pos(uint16_t dposition);
