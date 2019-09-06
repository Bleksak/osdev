[bits 32]
%define get_phys(x) x - 0xBFF00000

global load_page_directory
load_page_directory: ; arg => uintptr_t (address) of the directory
	mov eax, [esp+4]
	mov cr3, eax
	ret

global flush_tlb
extern page_directory

; flush_tlb:
; 	mov eax, get_phys(page_directory)
; 	mov cr3, eax
; 	ret

global readCR3
readCR3:
	mov eax, cr3
	ret