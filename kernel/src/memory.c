#include "memory.h"
#include "cpu/cpuid.h"

#include "asm.h"

extern void memcpy_sse(void* restrict dest, void* restrict src, size_t len);
extern void memcpy_normal(void* restrict dest, void* restrict src, size_t len);

void memcpy(void* restrict dest, void* restrict src, size_t len)
{
    if(!len)
        return;

    if(cpu_has_feature(CPUID_FEAT_SSE) && len > 127)
    {
        return memcpy_sse(dest, src, len);
    }

    return memcpy_normal(dest, src, len);
}

/*
void memset(void* dest, char ch, size_t len)
{
    uint8_t* _pDest = (uint8_t*) dest;

    while(len)
    {
        _pDest[--len] = ch;
    }
}
*/

bool memcmp(void* restrict dest, void* restrict src, size_t len)
{
    uint8_t* _dest = (uint8_t*) dest;
    uint8_t* _src = (uint8_t*) src;

    for(size_t i = 0; i<len; ++i)
    {
        if(_dest[i] != _src[i])
            return false;
    }

    return true;
}
