#include <stddef.h>
#include <stdint.h>

//Include necessary headers
// #include "memory.h"
// #include "ui.h"
// #include "shutdown.h"
// #include "snprintf.h"
#include "multiboot.h"

// Multiboot header structure
extern "C" __attribute__((section(".multiboot"), used))
const struct {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
} multiboot_header = {
    MULTIBOOT_HEADER_MAGIC,
    MULTIBOOT_HEADER_FLAGS,
    MULTIBOOT_HEADER_CHECKSUM
};

// Multiboot information structure provided by GRUB
// struct multiboot_info_t {
//     uint32_t flags;
//     uint32_t mem_lower;
//     uint32_t mem_upper;
//     // Other fields are omitted for simplicity
// };

// extern "C" {
//     // Ensure multiboot_info is defined in a writable section
//     __attribute__((section(".bss"))) multiboot_info_t* multiboot_info;
// }

// extern "C" void kernel_main() {
//     // Initialize the screen
//     init_screen();

//     // Display a welcome message
//     display_message("Memory Test OS v0.0.1");

//     // Get the total amount of memory
//     uintptr_t memory_start = 0x100000; // 1MB
//     uintptr_t memory_end = (multiboot_info->mem_upper * 1024); // Upper memory is in KB

//     // Display memory information
//     char buffer[256];
//     snprintf(buffer, sizeof(buffer), "Memory: %uKB lower, %uKB upper", multiboot_info->mem_lower, multiboot_info->mem_upper);
//     display_message(buffer);

//     // memory testing
//     display_message("Testing memory. Please wait!...");
//     bool memory_ok = test_memory(memory_start, memory_end);

//     if(memory_ok) display_message("Memory test passed.");
//     else display_message("Memory test failed");

//     // Display UI Elements
//     int total_memory_mb = (memory_end - memory_start) / (1024 * 1024);
//     int verified_memory_mb = total_memory_mb;
//     display_memory_status(verified_memory_mb, total_memory_mb);

//     // Main loop of the OS
//     int progress = 0;
//     int total = 100;
//     while(1) {
//         // Update UI
//         update_ui(progress, total);

//         // Handle input
//         char input = get_input();
//         process_input(input);

//         // Simulate progress
//         if(progress < total) progress++;
//         else progress = 0;

//         // Delay loop (simple busy-waiting for demonstration, replace with proper timing mechanisms)
//         for(volatile int i = 0; i < 1000000; ++i);
//     }
// }

void write_string(const char* str) {
    volatile char* video = (volatile char*) 0xB8000;
    while(*str) {
        *video++ = *str++;
        *video++ = 0x07;
    }
}

extern "C" void _start(uintptr_t mbi) {
    // Initialize multiboot_info
    // multiboot_info = reinterpret_cast<multiboot_info_t*>(mbi);
    // kernel_main();

    // DEBUG!
    extern char text_start, _end;
    // Print diagnostic information
    write_string("Boot successful\n");
    write_string("Text start: ");
    //Convert and print addresses
    write_string(reinterpret_cast<char*>(&text_start));
    write_string("\nEnd: ");
    write_string(reinterpret_cast<char*>(&_end));


    while(1) {
        asm volatile("hlt");
    }
}