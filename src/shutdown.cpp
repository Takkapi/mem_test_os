#include <stdint.h>

#include "shutdown.h"

// Function prototypes
uintptr_t find_rsdp();
bool validate_rsdp(uintptr_t rsdp);
uintptr_t find_rsdt(uintptr_t rsdp);
uintptr_t find_fadt(uintptr_t rsdt);
void outw(uint16_t port, uint16_t value);

// Constants for ACPI shutdown
#define ACPI_PM1_CNT_SLP_EN (1 << 13)
#define ACPI_PM1_CNT_SLP_TYP5 (5 << 10)

uintptr_t find_rsdp() {
    // Scan memory to find the RSDP (simplified example)
    for(uintptr_t address = 0xE0000; address < 0x100000; address += 16) {
        if(validate_rsdp(address)) 
            return address;
    }
    return 0;
}

bool validate_rsdp(uintptr_t rsdp) {
    // Validate the RSDP structure (simplified checksum example)
    const char* signature = "RSD PTR ";
    for(int i = 0; i < 8; i++) {
        if(((char*)rsdp)[i] != signature[i]) return false;
    }
    return true;
}

uintptr_t find_rsdt(uintptr_t rsdp) {
    // Extract the RSDT address fomr the RSDP
    return *(uintptr_t*)(rsdp + 16);
}

uintptr_t find_fadt(uintptr_t rsdt) {

}

static inline void outw(uint16_t port, uint16_t value) {
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

void shutdown() {
    // outw(ACPI_PM1a_CNT, (1 << 13));
    // if(ACPI_PM1b_CNT != 0)
    //     outw(ACPI_PM1b_CNT, (1 << 13));

    while(1) {
        asm volatile("hlt");
    }
}