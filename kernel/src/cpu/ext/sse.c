#include "sse.h"
#include "cpu/cpuid.h"

void _sse_enable(void);

void sse_enable(void)
{
    if(cpu_has_feature(CPUID_FEAT_SSE))
    {
        _sse_enable();
    }
}