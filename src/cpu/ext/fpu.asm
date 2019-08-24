bits 32

section .text
global _fpu_enable
_fpu_enable:
    fninit
    fnstsw [.testword]
    cmp word [.testword], 0
    setz al
    ret
    
section .data
.testword: dw 0x55AA