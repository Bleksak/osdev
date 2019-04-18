[bits 32]
%define get_phys(x) x - 0xBFF00000

global load_page_directory

load_page_directory:
	push ebp
	mov ebp, esp

	mov eax, [esp+8]
	mov cr3, eax
	
	mov esp, ebp
	pop ebp
	ret

global flush_tlb
extern page_directory

flush_tlb:
	mov eax, get_phys(page_directory)
	mov cr3, eax
	ret
