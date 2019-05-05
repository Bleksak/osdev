bits 32

global memset
memset:

    ;push eax
    ;push edi
    ;push ecx

    mov edi, [esp + 4] ; 16
    mov eax, [esp + 8] ; 20
    mov ecx, [esp + 12] ; 24
    
    rep stosb
    
    ;pop ecx
    ;pop edi
    ;pop eax

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
    push edx
    push esi
    push edi
    push ebx
    
    ;maybe dont need to save esi, edi and ebx because gcc already does ## see disasembly of memory.c ##
    ; actually i may need to edit the memory.c assembly output by hand

    mov esi, [esp + 28]
    mov edi, [esp + 24]
    mov ecx, [esp + 32]
    
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

    pop ebx
    pop edi
    pop esi
    pop edx
    pop ecx

    ret