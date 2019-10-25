#include "vga.h"
#include "../io.h"

uint16_t vga_width(void) {
    return 80;
}

void vga_cursor_enable(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void vga_cursor_disable(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

uint16_t vga_cursor_getpos(void) {
    outb(0x3D4, 0x0F);
    const uint16_t low = (uint16_t) inb(0x3D5);
    outb(0x3D4, 0x0E);
    const uint16_t high = (uint16_t) inb(0x3D5);

    return (high << 8) | low;
}

void vga_cursor_move_pos(uint16_t position) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, position & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (position >> 8) & 0xFF);
}

void vga_cursor_change(uint16_t dx, uint16_t dy) {
    const uint16_t ds = vga_width() * dy + dx;
    const uint16_t current_position = vga_cursor_getpos();
    const uint16_t new_position = current_position + ds;

    vga_cursor_move_pos(new_position);
}

void vga_cursor_change_pos(uint16_t dposition) {
    const uint16_t current_position = vga_cursor_getpos();
    const uint16_t new_position = current_position + dposition;

    vga_cursor_move_pos(new_position);
}

void vga_cursor_move(uint16_t x, uint16_t y) {
    const uint16_t new_position = y * vga_width() + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, new_position & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (new_position >> 8) & 0xFF);
}
