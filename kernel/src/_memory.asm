bits 32

global memset
memset:
    push ebp
    mov ebp, esp

    mov edi, [ebp+8]
    mov al, [ebp+12]
    mov ecx, [ebp+16]
    
    rep stosb

    mov esp, ebp
    pop ebp
    ret

global memcpy_sse
memcpy_sse:
    push ebp
    mov ebp, esp

    mov ecx, [ebp+16]
    mov esi, [ebp+12]
    mov edi, [ebp+8]
    mov edx, ecx

    and esi, 0xf
    mov esi, [ebp+12]
    jz .aligned
    
    mov ecx, esi 
    rep movsb

.aligned:
    mov ecx, edx
    and edx, 0xf
    shr ecx, 4

._loop:
    
    movaps xmm0, [esi]
    movntdq [edi], xmm0

    add esi, dword 16
    add edi, dword 16

    dec ecx
    jnz ._loop
.skip:
    xchg ecx, edx
    rep movsb

    mov esp, ebp
    pop ebp
    ret