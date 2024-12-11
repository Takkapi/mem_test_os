#include <stddef.h>
#include <stdint.h>

//Include necessary headers
#include "memory.h"
#include "ui.h"
#include "shutdown.h"
#include "snprintf.h"

struct multiboot_info_t {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    // Other fields are omitted for simplicity
};

// External symbols defined by the linker script
extern "C" multiboot_info_t* multiboot_info;

extern "C" void kernel_main() {
    // Initialize the screen
    init_screen();

    display_message("Memory Test v0.0.1");

    // Get the total amount of memory
    uintptr_t memory_start = 0x000000; // 0MB
    uintptr_t memory_end = (multiboot_info->mem_upper * 1024); // Upper memory is in KB

    // Display memory information
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Memory: %uKB lower, %uKB upper", multiboot_info->mem_lower, multiboot_info->mem_upper);
    display_message(buffer);

    display_message("Testing memory. Please wait!...");
    bool memory_ok = test_memory(memory_start, memory_end);

    if(memory_ok) display_message("Memory test passed.");
    else display_message("Memory test failed");

    // Display UI Elements
    int total_memory_mb = (memory_end - memory_start) / (1024 * 1024);
    int verified_memory_mb = total_memory_mb;
    display_memory_status(verified_memory_mb, total_memory_mb);

    // Main loop of the OS
    int progress = 0;
    int total = 100;
    while(1) {
        // Update UI
        update_ui(progress, total);

        // Handle input
        char input = get_input();
        process_input(input);

        // Simulate progress
        if(progress < total) progress++;
        else progress = 0;

        // Delay loop (simple busy-waiting for demonstration, replace with proper timing mechanisms)
        for(volatile int i = 0; i < 1000000; ++i);
    }
}

extern "C" void _start(multiboot_info_t* mbi) {
    multiboot_info = mbi;
    kernel_main();
}