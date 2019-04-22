bits 32

global memset
memset:
    mov edi, [esp+8]
    mov al, [esp+12]
    mov ecx, [esp+16]
    
    rep stosb
    ret

global memcpy_normal
memcpy_normal:
    mov esi, [esp + 8]
    mov edi, [esp + 4]
    mov ecx, [esp + 12]

    rep movsb
    ret

global memcpy_sse
memcpy_sse:

    push ecx
    push edx ;dont need to save esi, edi and ebx because gcc already does ## see disasembly of memory.c ##
    ; actually i may need to edit the assembly by hand

    mov esi, [esp + 16]
    mov edi, [esp + 12]
    mov ecx, [esp + 20]
    
    mov ebx, esi

    and ebx, 0xf
    sub esi, ebx
    sub ecx, ebx

    mov edx, ecx ; save the rest
    
    and edx, 15 ; edx => the rest which we will need to copy manually
    shr ecx, 4

.aligned:
.copy:
    movaps xmm0, [esi]
    movntdq [edi], xmm0

    add esi, 16
    add edi, 16
    dec ecx
    jnz .copy

    mov ecx, edx ;copy the rest
    rep movsb

    pop edx
    pop ecx

    ret