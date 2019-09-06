#pragma once

#include "../../std.h"


struct FxsaveBuffer {
    uint16_t fcw; // FPU control word
    uint16_t fsw; // FPU status word
    uint8_t ftw; // FPU tag word
    uint8_t rsvd; // 
    uint16_t fop; // FPU opcode
    uint32_t fpu_ip_low;
    uint16_t cs_fpu_ip_high;
    uint16_t _reserved1;
    uint32_t fpu_dp_low;
    uint16_t ds_fpu_dp_high;
    uint16_t _reserved2;
    uint32_t mxcsr;
    uint32_t mxcsr_mask;
    
    union {
        uint8_t MM0[10];
        uint8_t ST0[10];
    };
    
    uint8_t _reserved3[6];

    union {
        uint8_t MM1[10];
        uint8_t ST1[10];
    };
    
    uint8_t _reserved4[6];
    
    union {
        uint8_t MM2[10];
        uint8_t ST2[10];
    };
    
    uint8_t _reserved5[6];
    
    union {
        uint8_t MM3[10];
        uint8_t ST3[10];
    };
    
    uint8_t _reserved6[6];
    
    union {
        uint8_t MM4[10];
        uint8_t ST4[10];
    };
    
    uint8_t _reserved7[6];
    
    union {
        uint8_t MM5[10];
        uint8_t ST5[10];
    };
    
    uint8_t _reserved8[6];
    
    union {
        uint8_t MM6[10];
        uint8_t ST6[10];
    };
    
    uint8_t _reserved9[6];
    
    union {
        uint8_t MM7[10];
        uint8_t ST7[10];
    };

    uint8_t _reserved10[6];

    uint64_t xmm0[2];
    uint64_t xmm1[2];
    uint64_t xmm2[2];
    uint64_t xmm3[2];
    uint64_t xmm4[2];
    uint64_t xmm5[2];
    uint64_t xmm6[2];
    uint64_t xmm7[2];
    uint64_t xmm8[2];
    uint64_t xmm9[2];
    uint64_t xmm10[2];
    uint64_t xmm11[2];
    uint64_t xmm12[2];
    uint64_t xmm13[2];
    uint64_t xmm14[2];
    uint64_t xmm15[2];
} PACKED;

void sse_enable(void);
void fxsave(void* buffer);
void fxrstor(void* buffer);

uint16_t popcntw(uint16_t x);
uint32_t popcntl(uint32_t x);
uint64_t popcntll(uint64_t x);
