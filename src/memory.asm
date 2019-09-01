bits 32

; memset:                                 # @memset
;     push    esi
    
;     ; ecx should be counter, esi should be dest

;     mov     edx, dword ptr [esp + 16] ;counter
;     mov     eax, dword ptr [esp + 8] ; dest
;     imul    ecx, dword ptr [esp + 12], 0x01010101 ;ecx = char
;     mov     esi, edx
;     shr     esi, 2 ;esi = b32_c

;     je      .LBB0_1
; .l32:                                # =>This Inner Loop Header: Depth=1
;     mov     dword ptr [eax], ecx
;     add     eax, 4
;     dec     esi
;     jne     .LBB0_6 ;if b32_c == 0 gtfo
; .l16:
;     test    dl, 3
;     je      .LBB0_3
;     mov     word ptr [eax], cx
;     add     eax, 2
; .l8:
;     test    dl, 1
;     je      .LBB0_5
;     mov     byte ptr [eax], cl
; .LBB0_5:
;     pop     esi
;     ret

; global memset
; memset:

;     push eax
;     push edi
;     push ecx

;     mov edi, [esp + 4] ; 16
;     mov eax, [esp + 8] ; 20
;     mov ecx, [esp + 12] ; 24
    
;     rep stosb
    
;     pop ecx
;     pop edi
;     pop eax

;     ret

global memset
memset:
    push esi
    
    mov edi, dword [esp + 8]
    imul eax, dword [esp+ 12], 0x01010101
    mov ecx, dword [esp + 16]
    shr ecx, 2

    rep stosd
.bit16_transfer:
    test byte [esp + 16], 2
    jz .bit8_transfer

    stosw
.bit8_transfer:
    test byte [esp + 16], 1
    jz .done

    stosb

.done:
    pop esi
    ret

global memcpy_basic

global memcpy_basic_opt
memcpy_basic_opt:
    push esi
    push edi
    push ecx

    mov esi, [esp + 20]
    mov edi, [esp + 16]
    mov ecx, [esp + 24]

    shr ecx, 2
    rep movsb

.bit16_transfer:
    test byte [esp + 16], 2
    jz .bit8_transfer
    movsw
    
.bit8_transfer:
    test byte [esp + 16], 1
    jz .done
    movsb
    
.done:

    pop ecx
    pop edi
    pop esi

    ret

memcpy_basic:
    push esi
    push edi
    push ecx

    mov esi, [esp + 20]
    mov edi, [esp + 16]
    mov ecx, [esp + 24]

    rep movsb

    pop ecx
    pop esi
    pop edi
    ret

global memcpy_sse
memcpy_sse:

    push ecx
    push edx
    push esi
    push edi
    push ebx

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