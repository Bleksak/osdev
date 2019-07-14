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