#pragma once

#include <stdint.h>

uint16_t vga_width(void);

void vga_cursor_enable(uint8_t cursor_start, uint8_t cursor_end);
void vga_cursor_disable(void);
void vga_cursor_move(uint16_t x, uint16_t y);
void vga_cursor_change(uint16_t dx, uint16_t dy);
void vga_cursor_move_pos(uint16_t position);
void vga_cursor_change_pos(uint16_t dposition);

void vga_init(void);

struct VGAMode {
    uint8_t mode_ctl;
    uint8_t overscan;
    uint8_t color_plane_enable; // 4bits
    uint8_t horizontal_panning; // 4bits
    uint8_t color_select; // 4bits (is mostly 0)
    uint8_t misc_output;
    uint8_t clock_mode;
    uint8_t character_select;
    uint8_t memory_mode;
    uint8_t mode;
    uint8_t misc;
    uint8_t horizontal_total;
    uint8_t horizontal_display_enable_end;
    uint8_t horizontal_blank_start;
    uint8_t horizontal_blank_end;
    uint8_t horizontal_retrace_start;
    uint8_t horizontal_retrace_end;
    uint8_t vertical_total;
    uint8_t overflow;
    uint8_t preset_row_scan;
    uint8_t max_scanline;
    uint8_t vertical_retrace_start;
    uint8_t vertical_retrace_end;
    uint8_t vertical_display_enable_end;
    uint8_t logical_width;
    uint8_t underline_location;
    uint8_t vertical_blank_start;
    uint8_t vertical_blank_end;
    uint8_t mode_ctl2;
};

/*
    1280x1024@60Hz timings:
    General timing
    Screen refresh rate	60 Hz
    Vertical refresh	63.981042654028 kHz
    Pixel freq.	108.0 MHz
    Horizontal timing (line)
    Polarity of horizontal sync pulse is positive.
    Scanline part	Pixels	Time [µs]
    Visible area	1280	11.851851851852
    Front porch	48	0.44444444444444
    Sync pulse	112	1.037037037037
    Back porch	248	2.2962962962963
    Whole line	1688	15.62962962963

    Vertical timing (frame)
    Polarity of vertical sync pulse is positive.
    Frame part	Lines	Time [ms]
    Visible area	1024	16.004740740741
    Front porch	1	0.01562962962963
    Sync pulse	3	0.046888888888889
    Back porch	38	0.59392592592593
    Whole frame	1066	16.661185185185
*/
