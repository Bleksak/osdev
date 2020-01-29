%define get_phys(x) (x) - 0xBFF00000
%define get_virt(x) (x) + 0xBFF00000


bits 32
; Declare constants for the multiboot header.
MBALIGN   equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO   equ  1 << 1            ; provide memory map
VGA		  equ  1 << 2

WIDTH	  equ  0
HEIGHT    equ  0
DEPTH	  equ  0
MODE_TYPE equ  1 ; 0 = graphics, 1 => text

FLAGS     equ  MBALIGN | MEMINFO | VGA ; this is the Multiboot 'flag' field
MAGIC     equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM  equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot 

; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd MODE_TYPE
	dd WIDTH
	dd HEIGHT
	dd DEPTH
; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
section .bss

extern page_directory, page_table

align 4096
stack_bottom:
resb 32768 ; 32 KiB
stack_top:

; align 0x1000
; page_directory:
; resb 0x1000
; align 0x1000
; page_table:
; resb 0x1000*0x1000
; .end:
 
; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
; Declare _start as a function symbol with the given symbol size.
section .text
extern _kernel_phys_start, _kernel_phys_end

global _start
_start:

	cmp eax, 0x2badb002
	jnz get_phys(no_multiboot)

	xor eax, eax
	mov edi, get_phys(page_directory)
	mov ecx, 0x1000
	
	rep movsb

	mov edi, get_phys(page_table)
	mov ecx, 0x1000*0x1000

	rep movsb

	mov esi, 0x1000
	mov edi, get_phys(page_table) + 4
.l1:

	mov eax, esi
	or eax, 3
	mov [edi], eax

	add esi, 0x1000
	add edi, 4

	cmp edi, get_phys(page_table) + 0x1000 * 0x1000
	jb .l1

	mov esi, _kernel_phys_start
	
	mov edi, get_phys(page_table) + 768 * 0x1000

.l2:

	mov eax, esi
	or eax, 3
	mov [edi], eax

	add esi, 0x1000
	add edi, 4

	cmp esi, _kernel_phys_end
	jb .l2


	mov esi, get_phys(page_table)
	mov edi, get_phys(page_directory)

.l3:
	mov eax, esi
	or eax, 3
	mov [edi], eax

	add esi, 0x1000
	add edi, 4

	cmp edi, get_phys(page_directory) + 0x1000
	jb .l3

	mov eax, get_phys(page_directory)
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax

	mov esp, stack_top

	push ebx

	extern kernel_main
	call get_virt(kernel_main)
	
	; If the system has nothing more to do, put the computer into an
	; infinite loop. To do that:
	; 1) Disable interrupts with cli (clear interrupt enable in eflags).
	;    They are already disabled by the bootloader, so this is not needed.
	;    Mind that you might later enable interrupts and return from
	;    kernel_main (which is sort of nonsensical to do).
	; 2) Wait for the next interrupt to arrive with hlt (halt instruction).
	;    Since they are disabled, this will lock up the computer.
	; 3) Jump to the hlt instruction if it ever wakes up due to a
	;    non-maskable interrupt occurring or due to system management mode.
	
.hang: 
	cli
	hlt
	jmp .hang
.end:


no_multiboot:
	ret