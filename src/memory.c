#include "memory.h"
#include "cpu/cpuid.h"

extern void memcpy_sse(void* restrict dest, void* restrict src, size_t len);
extern void memcpy_basic(void* restrict dest, void* restrict src, size_t len);
extern void memcpy_basic_opt(void* restrict dest, void* restrict src, size_t len);

void memcpy(void* restrict dest, void* restrict src, size_t len)
{
    if(!len)
        return;

    if(cpu_has_feature(CPUID_FEAT_SSE) && len > 127)
    {
        return memcpy_sse(dest, src, len);
    }

    return memcpy_basic_opt(dest, src, len);
}

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
