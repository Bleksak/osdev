OUTPUT_FORMAT(elf32-i386)
/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 0xC0000000;

	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */

	_kernel_virt_start = .;
	_kernel_phys_start = _kernel_virt_start - 0xBFF00000;

	.text ALIGN (4K) : AT (ADDR (.text) - 0xBFF00000)
	{
		_TEXT_START = .;
		*(.multiboot)
		*(.text)
		_TEXT_END = .;
	}
 
	/* Read-only data. */
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