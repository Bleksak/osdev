#include "vga.h"
#include <io.h>

extern volatile short* vga_buffer;

enum VGA_REGISTERS {
    VGA_REG_ATTRIBUTE_ADDR = 0x3C0,
    VGA_REG_ATTRIBUTE_DATA,
    VGA_REG_MISC_OUTPUT,
    VGA_REG_INPUT_STATUS0 = 0x3C2,
    VGA_REG_SEQUENCER_ADDR = 0x3C4,
    VGA_REG_SEQUENCER_DATA,
    VGA_REG_DAC_STATE = 0x3C7,
    VGA_REG_DAC_ADDR_READ_MODE = 0x3C7,
    VGA_REG_DAC_ADDR_WRITE_MODE,
    VGA_REG_DAC_DATA,
    VGA_REG_FEATURE_CONTROL,
    VGA_REG_MISC_OUTPUT_READ = 0x3CC,
    VGA_REG_GRAPHICS_ADDR = 0x3CE,
    VGA_REG_GRAPHICS_DATA,
    VGA_REG_CRTC_ADDRESS = 0x3D4,
    VGA_REG_CRTC_DATA
};

enum VGA_MODES {
    ATGE = 1 << 0,
    MONO = 1 << 1,
    LGE = 1 << 2,
    BLINK = 1 << 3,
    PPM = 1 << 5,
    BIT8 = 1 << 6,
    P54S = 1 << 7,
};

struct VGAMode vga_mode_640x480x16 = {
    .mode_ctl = 0x01,
    .overscan = 0x00,
    .color_plane_enable = 0x0F,
    .horizontal_panning = 0x00,
    .color_select = 0x00,
    .misc_output = 0xE3,
    .clock_mode = 0x01,
    .character_select = 0x00,
    .memory_mode = 0x02,
    .mode = 0x00,
    .misc = 0x05,
    .horizontal_total = 0x5F,
    .horizontal_display_enable_end = 0x4F,
    .horizontal_blank_start = 0x50,
    .horizontal_blank_end = 0x82,
    .horizontal_retrace_start = 0x54,
    .horizontal_retrace_end = 0x80,
    .vertical_total = 0x0B,
    .overflow = 0x3E,
    .preset_row_scan = 0x00,
    .max_scanline = 0x40,
    .vertical_retrace_start = 0xEA,
    .vertical_retrace_end = 0x8C,
    .vertical_display_enable_end = 0xDF,
    .logical_width = 0x28,
    .underline_location = 0x00,
    .vertical_blank_start = 0xE7,
    .vertical_blank_end = 0x04,
    .mode_ctl2 = 0xE3,
};

static void vga_set_modectl(uint8_t modectl) {
    outb(VGA_REG_ATTRIBUTE_ADDR, 0x10);
    outb(VGA_REG_ATTRIBUTE_DATA, modectl);
}

static void vga_set_overscan(uint8_t value) {
    outb(VGA_REG_ATTRIBUTE_ADDR, 0x11);
    outb(VGA_REG_ATTRIBUTE_DATA, value);
}

static void vga_set_color_plane_enable(uint8_t value) {
    outb(VGA_REG_ATTRIBUTE_ADDR, 0x12);
    outb(VGA_REG_ATTRIBUTE_DATA, value);
}

static void vga_set_color_select(uint8_t value) {
    outb(VGA_REG_ATTRIBUTE_ADDR, 0x14);
    outb(VGA_REG_ATTRIBUTE_DATA, value);
}

static void vga_misc_write(uint8_t value) {
    outb(VGA_REG_MISC_OUTPUT, value);
}

static void vga_set_clock_mode(uint8_t value) {
    outb(VGA_REG_SEQUENCER_ADDR, 0x01);
    outb(VGA_REG_SEQUENCER_DATA, value);
}

static void vga_set_character_select(uint8_t value) {
    outb(VGA_REG_SEQUENCER_ADDR, 0x03);
    outb(VGA_REG_SEQUENCER_DATA, value);
}

static void vga_set_memory_mode(uint8_t value) {
    outb(VGA_REG_SEQUENCER_ADDR, 0x04);
    outb(VGA_REG_SEQUENCER_DATA, value);
}

static void vga_set_mode(uint8_t value) {
    outb(VGA_REG_GRAPHICS_ADDR, 0x05);
    outb(VGA_REG_GRAPHICS_DATA, value);
}

static void vga_set_misc(uint8_t value) {
    outb(VGA_REG_GRAPHICS_ADDR, 0x06);
    outb(VGA_REG_GRAPHICS_DATA, value);
}

static void vga_set_horizontal_panning(uint8_t value) {
    outb(VGA_REG_ATTRIBUTE_ADDR, 0x13);
    outb(VGA_REG_ATTRIBUTE_DATA, value);
}

static void vga_set_horizontal_total(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0); // horizontal_total register
    outb(VGA_REG_CRTC_DATA, value); // subtract 5 (maybe)
}

static void vga_set_horizontal_display_enable_end(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x0);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_horizontal_blank_start(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x02);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_horizontal_blank_end(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x03);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_horizontal_retrace_start(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x04);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_horizontal_retrace_end(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x05);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_vertical_total(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x06);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_overflow_register(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x07);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_preset_row_scan(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x08);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_max_scanline(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x09);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_vertical_retrace_start(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x10);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_vertical_retrace_end(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x11);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_vertical_display_enable_end(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x12);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_logical_width(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x13);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_underline_location(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x14);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_vertical_blank_start(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x15);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_vertical_blank_end(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x16);
    outb(VGA_REG_CRTC_DATA, value);
}

static void vga_set_mode_control(uint8_t value) {
    outb(VGA_REG_CRTC_ADDRESS, 0x17);
    outb(VGA_REG_CRTC_DATA, value);
}

void vga_load_mode(const struct VGAMode* mode) {
    vga_set_modectl(mode->mode_ctl);
    vga_set_overscan(mode->overscan);
    // vga_set_color_plane_enable(mode->color_plane_enable);
    // vga_set_horizontal_panning(mode->horizontal_panning);
    // vga_set_color_select(mode->color_select);
    // vga_misc_write(mode->misc_output);
    // vga_set_clock_mode(mode->clock_mode);
    // vga_set_character_select(mode->character_select);
    // vga_set_memory_mode(mode->memory_mode);
    // vga_set_mode(mode->mode);
    // vga_set_misc(mode->misc);
    // vga_set_horizontal_total(mode->horizontal_total);
    // vga_set_horizontal_display_enable_end(mode->horizontal_display_enable_end);
    // vga_set_horizontal_blank_start(mode->horizontal_blank_start);
    // vga_set_horizontal_blank_end(mode->horizontal_blank_end);
    // vga_set_horizontal_retrace_start(mode->horizontal_retrace_start);
    // vga_set_horizontal_retrace_end(mode->horizontal_retrace_end);
    // vga_set_vertical_total(mode->vertical_total);
    // vga_set_overflow_register(mode->overflow);
    // vga_set_preset_row_scan(mode->preset_row_scan);
    // vga_set_max_scanline(mode->max_scanline);
    vga_set_vertical_retrace_start(mode->vertical_retrace_start);
    vga_set_vertical_retrace_end(mode->vertical_retrace_end);
    vga_set_vertical_display_enable_end(mode->vertical_display_enable_end);
    vga_set_logical_width(mode->logical_width);
    vga_set_underline_location(mode->underline_location);
    vga_set_vertical_blank_start(mode->vertical_blank_start);
    vga_set_vertical_blank_end(mode->vertical_blank_end);
    vga_set_mode_control(mode->mode_ctl2);

    // vga_buffer = (volatile short*) 0xA0000;

    // for(int i = 0; i < 1024; ++i) {
    //     vga_buffer[i] = 0xff;
    // }

}

void vga_init(void) {
    vga_load_mode(&vga_mode_640x480x16);
}

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
