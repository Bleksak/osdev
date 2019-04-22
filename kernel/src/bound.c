#include "bound.h"

__attribute__((pure)) bool bound(uintptr_t start, uintptr_t end, uintptr_t start_addr, uintptr_t end_addr)
{
    if(start < start_addr)
    {
        return (end >= start_addr && end <= end_addr);
    }

    if(start > start_addr)
    {
        return (start <= end_addr && end <= end_addr);
    }

    // If we're equal we have nothing to talk about
    return true;
}

__attribute__((pure)) bool bound_s(uintptr_t start, uintptr_t end, uintptr_t addr)
{   
    if(start < addr)
    {
        return (end >= addr);
    }

    return true;
}