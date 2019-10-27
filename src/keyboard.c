#include "keyboard.h"
#include "isr.h"
#include "console.h"
#include "gfx/vga.h"

typedef void(*keyboard_func_handler)(unsigned char scancode, bool make);

static bool ext = false;

#define KEY_ESCAPE 0x1
#define KEY_TAB 0xF
#define KEY_CAPS 0x3A
#define KEY_LSHIFT 0x2A
#define KEY_LCTRL 0x1D
#define KEY_LALT 0x38
#define KEY_BACKSPACE 0xE
#define KEY_NUMLOCK 0x45
#define KEY_RSHIFT 0x36

#define KEY_EXT 0xE0
#define KEY_RELEASE 0x80
#define KEY_INSERT 0x52
#define KEY_HOME 0x47
#define KEY_PGUP 0x49
#define KEY_PGDN 0x51
#define KEY_DELETE 0x53
#define KEY_END 0x4F
#define KEY_UP 0x48
#define KEY_LEFT 0x4B
#define KEY_DOWN 0x50
#define KEY_RIGHT 0x4D
#define KEY_SLASH 0x35
#define KEY_RCTRL 0x1D
#define KEY_RGUI 0x5B
#define KEY_RALT 0x38
#define KEY_APPS 0x5D

enum KeyType {
  KEY_CHARACTER,
  KEY_FUNCTION,
  KEY_UNDEFINED,
};

struct Key {
  enum KeyType type;
  union {
    keyboard_func_handler handler;
    unsigned int ascii_value;
  };
};

static const unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

static void keyboard_escape_handler(unsigned char scancode, bool make);
static void keyboard_shift_handler(unsigned char scancode, bool make);


static void keyboard_insert_handler(unsigned char scancode, bool make) {
	(void) scancode;
	if(make) {
		printf("Clipboard pasted\n");
	}
}

static void keyboard_up_handler(unsigned char scancode, bool make);
static void keyboard_down_handler(unsigned char scancode, bool make);
static void keyboard_left_handler(unsigned char scancode, bool make);
static void keyboard_right_handler(unsigned char scancode, bool make);
static void keyboard_backspace_handler(unsigned char scancode, bool make);

static const struct Key us_keymap[256] = {
	{
		KEY_UNDEFINED,
		{0}
	},

	{
		KEY_FUNCTION,
		{keyboard_escape_handler},
	},
  
	[KEY_LSHIFT] = {
		KEY_FUNCTION,
		{keyboard_shift_handler},
	},

	[KEY_BACKSPACE] = {
		KEY_FUNCTION,
		{keyboard_backspace_handler},
	},
};

static const struct Key ext_keymap[] = {
	[KEY_INSERT] = {
		KEY_FUNCTION,
		{keyboard_insert_handler},
	},

	[KEY_UP] = {
		KEY_FUNCTION,
		{keyboard_up_handler},
	},
	[KEY_DOWN] = {
		KEY_FUNCTION,
		{keyboard_down_handler},
	},

	[KEY_LEFT] = {
		KEY_FUNCTION,
		{keyboard_left_handler},
	},

	[KEY_RIGHT] = {
		KEY_FUNCTION,
		{keyboard_right_handler},
	}
};

static bool shift = false;
static bool caps_lock = false;

static char scancode_to_ascii(unsigned char scancode) {
	return kbdus[scancode];
}

static void keyboard_handle_default(unsigned char scancode, bool make) {
	if(!make) {
		return;
	}

	char key = scancode_to_ascii(scancode);

	/*
	caps  shift   lower
	0     0       1
	0     1       0
	1     0       0
	1     1       1
	*/

	if(key >= 'A' && key <= 'Z') {
		key |= (shift == caps_lock) << 5;
	}

	putch(key);
	update_cursor();
}

static void keyboard_backspace_handler(unsigned char scancode, bool make) {
	if(make) {
		console_erase_back();
	}
}

static void keyboard_shift_handler(unsigned char scancode, bool make) {
	(void) scancode;
	shift = make;
}

static void keyboard_escape_handler(unsigned char scancode, bool make) {
	(void) scancode;
	if(make) {
		printf("Escape pressed\n");
	}
}

static void keyboard_right_handler(unsigned char scancode, bool make) {
	(void) scancode;
	(void) make;
}

static void keyboard_up_handler(unsigned char scancode, bool make) {
	(void) scancode;
	(void) make;
}

static void keyboard_left_handler(unsigned char scancode, bool make) {
	(void) scancode;
	(void) make;

	console_back();
}

static void keyboard_down_handler(unsigned char scancode, bool make) {
	(void) scancode;
	(void) make;
}

void keyboard_handle_caps_lock(unsigned char scancode) {
	if(scancode & ~KEY_RELEASE) {
		caps_lock = (~caps_lock) & 1;
	}
}

static bool key_is_make(unsigned char scancode) {
	return !((scancode & KEY_RELEASE) >> 7);
}

static void keyboard_handle(struct ISRRegisters* regs) {
	(void) regs;
	unsigned char scancode = inw(0x60);
	unsigned char purecode = scancode & ~KEY_RELEASE;

	bool make = key_is_make(scancode);
	
	if(scancode == KEY_EXT) {
		ext = true;
		return;
	}

	if(ext) {
		if(ext_keymap[purecode].type == KEY_FUNCTION) {
			ext_keymap[purecode].handler(scancode, make);
		}

		ext = false;
		return;
	}

	if(us_keymap[purecode].type == KEY_FUNCTION) {
		us_keymap[purecode].handler(scancode, make);
		return;
	}

	keyboard_handle_default(scancode, make);
}

void keyboard_install(void) {
    irq_install_handler(1, keyboard_handle);
}

#include "acpi/ioapic.h"

extern void dummy(void);

void keyboard_install_apic() {
	interrupt_set_base(0x30, dummy);
	ioapic_enable_irq(1, 0x30);
}