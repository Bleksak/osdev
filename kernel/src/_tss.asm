bits 32
global enterUserMode
global test_func

test_func:
.stack_fix:
    add esp, 4
.done:
    ; int 0x10
    jmp .done

enterUserMode:
    ; mov ebx, [esp + 4]

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov gs, ax

    mov eax, esp
    push 0x23
    push eax
    pushf
    push 0x1B
    lea eax, [test_func]
    push eax  ; the function address
    iretd ; return from an "interrupt"