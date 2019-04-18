extern madt

%define APIC_APICID	    20h
%define APIC_APICVER	30h
%define APIC_TASKPRIOR	80h
%define APIC_EOI	    0B0h
%define APIC_LDR	    0D0h
%define APIC_DFR	    0E0h
%define APIC_SPURIOUS	0F0h
%define APIC_ESR	    280h
%define APIC_ICRL	    300h
%define APIC_ICRH	    310h
%define APIC_LVT_TMR	320h
%define APIC_LVT_PERF	340h
%define APIC_LVT_LINT0	350h
%define APIC_LVT_LINT1	360h
%define APIC_LVT_ERR	370h
%define APIC_TMRINITCNT	380h
%define APIC_TMRCURRCNT	390h
%define APIC_TMRDIV	    3E0h
%define APIC_LAST	    38Fh
%define APIC_DISABLE	10000h
%define APIC_SW_ENABLE	100h
%define APIC_CPUFOCUS	200h
%define APIC_NMI	    (4<<8)
%define TMR_PERIODIC	20000h
%define TMR_BASEDIV	    (1<<20)

%define apic_msr 0x1B

section .data
global apic_addr, io_apic_addr
apic_addr: dd 0
io_apic_addr: dd 0

section .text

cpu_set_apic_msr:
    mov ecx, apic_msr

    rdmsr
    bts eax, 11
    wrmsr

    ret