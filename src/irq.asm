bits 32

extern irq_handler

irq_common_stub:
    pushad
    push ds
    push es
    push fs
    push gs
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push eax
    call irq_handler
    
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popad

    add esp, 4
    iretd

; irq1:
;     cli
;     push dword 33
    
;     jmp irq_common_stub


%macro irq 1
    global irq%1
    irq%1:
        cli
        push dword 32+%1
        jmp irq_common_stub
%endmacro

%assign i 0

%rep 16
    irq i
    %assign i i+1
%endrep
