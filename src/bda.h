#pragma once

struct BiosDataArea {
    unsigned char padding[0x400];

    unsigned short com_ptr[4];
    unsigned short lpt_ptr[3];
    unsigned short ebda_ptr;

    struct {
        /*
            maybe reverse the order, not sure
        */

        unsigned char parallel_ports : 2;
        /*
            00b => 1 parallel port
            01 => 2 parallel ports,
            11 => 3 parallel ports,
        */
        unsigned char _reserved1 : 2;

        unsigned char serial_ports : 3;
        /*
            000 => none
            001 => 1
            002 => 2
            003 => 3
            004 => 4
        */

        unsigned char _reserved2 : 1;

        unsigned char floppy_drives : 2;
        /*
            00 => 1
            01 => 2
        */

       unsigned char video_mode : 2;
       /*
            0 => EGA or later
            01 => color 40x25
            10 => color 80x25
            11 => monochrome 80x25
       */
       unsigned char reserved3 : 1;
       unsigned char ps2_mouse : 1; // 1 => installed, 0 => none
       unsigned char math_coprocessor : 1;
       unsigned char boot_floppy : 1;
    } __attribute__((packed)) equipment;

    unsigned char interrupt_flag;
    unsigned short memory_size; // in Kb
    unsigned short unused1;

    struct {
        unsigned char insert : 1;
        unsigned char caps_lock : 1;
        unsigned char num_lock : 1;
        unsigned char scroll_lock : 1;
        unsigned char alt_key : 1; // 0 => up, 1 => down
        unsigned char control : 1;
        unsigned char left_shift : 1;
        unsigned char right_shift : 1;
    } __attribute__((packed)) keyboard_flags;

    struct {
        unsigned char insert : 1; // up or down
        unsigned char caps_lock : 1;
        unsigned char num_lock : 1;
        unsigned char scroll_lock : 1;
        unsigned char pause_key : 1; // active or inactive
        unsigned char sysreg_key : 1;
        unsigned char left_alt : 1;
        unsigned char right_alt : 1;
    } __attribute__((packed)) keyboard_flags2;

    unsigned char alt_numpad_area;
    
    struct {
        unsigned short next_ptr;
        unsigned short last_ptr;
        unsigned char buffer[32];
    } __attribute__((packed)) keyboard_buffer;

    unsigned char unused2[11]; //floppy stuff

    struct {
        unsigned char active_mode_setting;
        unsigned short text_columns_per_row;
        unsigned short active_video_size; // in page bytes (not sure what it means)
        unsigned short video_page_offset;
        unsigned short cursor_positions[8];
        unsigned short cursor_shape;
        unsigned char active_page;
        unsigned short display_adapter_io_port;
        
        struct {
            unsigned char unused1 : 2;
            unsigned char controls : 1; // 0 background intensity, 1 blinking
            unsigned char graphics_operation : 1;
            unsigned char video_signal : 1; // enabled/disabled
            unsigned char color_operation : 1; // 0 => color, 1 => monochrome
            unsigned char unused2 : 2;
        } __attribute__((packed)) internal_mode_register;
        
        unsigned char color_palette;
        unsigned short adapter_rom_offset;
        unsigned short adapter_rom_segment;
    } __attribute__((packed)) video_mode;

    unsigned char last_interrupt;
    unsigned int useless_counter;
    unsigned char timer24_hour_flag;
    unsigned char keyboard_ctrl_break_flag;
    unsigned short soft_reset_flag;
    
    struct {
        unsigned char last_operation_status;
        unsigned char hard_drive_count;
        unsigned char drive_control_byte;
        unsigned char drive_io_port_offset;
    } __attribute__((packed)) drives;

    unsigned char unused3[12];

    unsigned char video_row_count; // minus 1
    unsigned char unused4[19];

    struct {
        unsigned int flag_ptr; //Segment:Offset
        unsigned int count;
        unsigned char flag;
    } __attribute__((packed)) user_wait;

    unsigned char lan_bytes[7];
    unsigned int video_paramter_control_block_ptr; //Segment:Offset
    unsigned char _reserved3[68];
    unsigned char communications_area[16];
} __attribute__((packed)) ;

typedef struct BiosDataArea BDA;

const BDA* bda_get(void);

void bda_init(void);
