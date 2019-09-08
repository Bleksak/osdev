#include "bound.h"

#include "console.h"

bool bound(uintptr_t start, uintptr_t end, uintptr_t start_addr, uintptr_t end_addr) {
    if(start_addr < start) { // start_addr | start | ............... | 
                             // start_addr|end_addr|start|end
        return (end_addr >= start); // start_addr|start|end_addr|end
                                    // start_addr|start|end|end_addr
    }

    if(start_addr > start) { // start|start_addr|.....
                             // start|end|start_addr|end_addr....
        return (start_addr <= end); // start| start_addr | end_addr | end
                                    // start| start_addr | end | end_addr
    }
    
    return true;
}

bool bound_s(uintptr_t start, uintptr_t end, uintptr_t addr) {   
    if(start < addr) {
        return (end >= addr);
    }

    return true;
}
