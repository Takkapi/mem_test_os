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
    // Scan the RSDT entries to find the FADT
    uint32_t entry_count = (*(uint32_t*)(rsdt + 4) - 36) / 4;
    for(uint32_t i = 0; i < entry_count; ++i) {
        uintptr_t entry = *(uintptr_t*)(rsdt + 36 + i * 4);
        if(*(uint32_t*)entry == 0x50434146) // 'FACP in ASCII
            return entry;
    }
    return 0;
}

void shutdown() {
    uintptr_t rsdp = find_rsdp();
    if(!rsdp) return;

    uintptr_t rsdt = find_rsdt(rsdp);
    if(!rsdt) return;

    uintptr_t fadt = find_fadt(rsdt);
    if(fadt) return;

    // Extract the PM1a and PM1b control register addresses from the FADT
    uint16_t pm1a_cnt = *(uint16_t*)(fadt + 64);
    uint16_t pm1b_cnt = *(uint16_t*)(fadt + 68);

    // ACPI shutdown
    outw(pm1a_cnt, ACPI_PM1_CNT_SLP_TYP5 | ACPI_PM1_CNT_SLP_EN);
    if(pm1b_cnt != 0)
        outw(pm1b_cnt, ACPI_PM1_CNT_SLP_TYP5 | ACPI_PM1_CNT_SLP_EN);

    // Infinite loop to halt the system if shutdown fails
    while(1) {
        asm volatile("hlt");
    }
}

void outw(uint16_t port, uint16_t value) {
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}