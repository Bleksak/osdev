OUTPUT_FORMAT(elf32-i386)

ENTRY(_start)

SECTIONS
{
	. = 0xC0000000;

	_kernel_virt_start = .;
	_kernel_phys_start = _kernel_virt_start - 0xBFF00000;

	.text ALIGN (4K) : AT (ADDR (.text) - 0xBFF00000)
	{
		_TEXT_START = .;
		*(.multiboot)
		*(.text)
		_TEXT_END = .;
	}
 
	.rodata ALIGN(4K) : AT (ADDR (.rodata) - 0xBFF00000)
	{
		_RODATA_START = .;
		*(.rodata)
		_RODATA_END = .;
	}
 
	/* Read-write data (initialized) */
	
	.data ALIGN(4K) : AT (ADDR (.data) - 0xBFF00000)
	{
		_DATA_START = .;
		*(.data)
		_DATA_END = .;
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT (ADDR (.bss) - 0xBFF00000)
	{
		_BSS_START = .;
		*(COMMON)
		*(.bss)
		_BSS_END = .;
	}

	_kernel_virt_end = .;
	_kernel_phys_end = _kernel_virt_end - 0xBFF00000;

	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}