bits 32

global enable_apic

enable_apic:
    push eax
    push ecx
    push edx
    pushf

    mov ecx, 0x1B
    rdmsr
    bts eax, 11
    wrmsr
    
    popf
    pop edx
    pop ecx
    pop eax
    
    ret

global dummy, spurious
extern lapic_base, puts

asdf: db "nasrat", 0

dummy:
    cli
    push asdf
    call puts
    add esp, 4
    mov dword [lapic_base + 0xB0], 0
spurious:
    mov dword [lapic_base + 0xB0], 0
    iret
