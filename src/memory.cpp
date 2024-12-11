#include <stdint.h>

bool test_memory(uintptr_t start, uintptr_t end) {
    for(uintptr_t address = start; address < end; address += sizeof(uintptr_t)) {
        uintptr_t value = *(volatile uintptr_t*)address;
        *(volatile uintptr_t*)address = ~value;
        
        if(*(volatile uintptr_t*)address != ~value)
            return false;
        
        *(volatile uintptr_t*)address = value;
    }

    return true;
}