bits 32

global cpuid_available
cpuid_available:
    pushfd
    pushfd
    xor dword [esp], 0x00200000
    popfd
    pushfd
    pop eax
    xor eax, [esp]
    popfd
    and eax, 0x00200000
    shr eax, 20
    ret

global enable_sse
enable_sse:
    mov eax, cr0
    and ax, 0xfffb
    or ax, 2
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9
    mov cr4, eax
    ret