bits 32

section .text

global _isr_handler
extern isr_handler

global _exception_handler
extern exception_handler

%assign i 0

%macro isr 2
    global isr%1
    isr%1:
        cli
        push dword i
        jmp %2
    %assign i i+1
%endmacro

%rep 8
    isr i, _isr_handler
%endrep
    isr i, _exception_handler
    isr i, _isr_handler
%rep 5
    isr i, _exception_handler
%endrep
%rep 17
    isr i, _isr_handler
%endrep

_isr_handler:
    pushad
    push ds
    push es
    push gs
    push fs

    mov eax, 0x10
    mov ds, eax
    mov es, eax
    mov gs, eax
    mov fs, eax

    push esp
    call isr_handler
    pop esp

    pop fs
    pop gs
    pop es
    pop ds
    popad

    add esp, 4
    
    iretd           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

_exception_handler:
    pushad
    push ds
    push es
    push gs
    push fs

    mov eax, 0x10
    mov ds, eax
    mov es, eax
    mov gs, eax
    mov fs, eax

    push esp
    call exception_handler
    pop esp

    pop fs
    pop gs
    pop es
    pop ds
    popad

    add esp, 8
    
    iretd           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!