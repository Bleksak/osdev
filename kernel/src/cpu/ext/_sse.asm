global _sse_enable
_sse_enable:
    mov eax, cr0
    and ax, 0xfffb
    or ax, 2
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9
    mov cr4, eax
    ret