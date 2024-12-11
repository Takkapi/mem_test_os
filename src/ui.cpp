#include <stdint.h>

#include "ui.h"
#include "shutdown.h"
#include "snprintf.h"

// Address of VGA text buffer 
#define VGA_ADDRESS 0xB8000 
#define VGA_WIDTH 80 
#define VGA_HEIGHT 25

//VGA color codes
enum vga_color {
    COLOR_BLACK = 0,
    COLOR_WHITE = 15,
};

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;

// Function to set a character and its color a specific position in VGA text mode
void set_vga_entry(int x, int y, char c, uint8_t color) {
    const int index = y * VGA_WIDTH + x;
    vga_buffer[index] = ((uint16_t)color << 8) | (uint8_t)c;
}

// Function to initialize the screen (clear the screen)
void init_screen() {
    uint8_t color = (COLOR_BLACK << 4) || COLOR_WHITE; // Black backgroup, white text
    for(int y = 0; y < VGA_HEIGHT; ++y) {
        for(int x = 0; x < VGA_WIDTH; ++x) {
            set_vga_entry(x, y, ' ', color);
        }
    }
}

// Function to display a message on the screen
void display_message(const char* message) {
    static int cursor_y = 0;
    static int cursor_x = 0;
    uint8_t color = (COLOR_BLACK << 4) | COLOR_WHITE;

    for(size_t i = 0; message[i] != '\0'; ++i) {
        if(message[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else {
            set_vga_entry(cursor_x, cursor_y, message[i], color);
            cursor_x++;
            if(cursor_x >= VGA_WIDTH) {
                cursor_x = 0;
                cursor_y++;
            }
        }
        
        if(cursor_y >= VGA_HEIGHT) {
            // Scroll the screen up if we reach the bottom
            for(int y = 0; y < VGA_HEIGHT - 1; ++y) {
                for(int x = 0; x < VGA_WIDTH; ++x) {
                    vga_buffer[y * VGA_WIDTH - 1, ' ', color];
                }
                cursor_y = VGA_HEIGHT - 1;
            }
        }
    }
}

// Function to display memory status
void display_memory_status(int verified, int total) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Memory: %uMB lower, %uMB upper", verified, total);
    display_message(buffer);
}

// Function to display a progress bar on the screen
void display_progress_bar(int x, int y, int progress, int total) {
    uint8_t color = (COLOR_BLACK << 4) | COLOR_WHITE; // Black background, white text
    int bar_width = VGA_WIDTH - x - 2; // Leave space for borders
    int filled_width = (progress * bar_width) / total;

    // Draw the progress bar
    set_vga_entry(x, y, '[]', color);
    for(int i = 0; i < bar_width; ++i) {
        if(i < filled_width) set_vga_entry(x + 1 + i, y, '#', color);
        else set_vga_entry(x + 1 + i, y, ' ', color);
    }
    
    set_vga_entry(x + 1 + bar_width, y, ']', color);
}

// Function to update the UI periodically
void update_ui(int progress, int total) {
    // Update the progress bar (example poisition, x=0, y=24)
    display_progress_bar(0, 24, progress, total);
}

// read a byte from the specified port
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Check if a keyboard input is available
bool is_key_available() {
    return inb(0x64) & 0x01;
}

// Get input from the keyboard
char get_input() {
    if(is_key_available()) {
        uint8_t scan_code = inb(0x60);
        if(scan_code & 0x80) {
            // Key release event (ignore for simplicity)
            return '\0';
        } else {
            // Key presse event
            switch(scan_code) {
                case 0x10: return 'q'; // Scan code for 'q'
                // Add more scan code as needed
                default: return '\0'; // Unknown key
            }
        }
    }
    return '\0'; // No key pressed
}

void process_input(char input) {
    if(input == 'q') shutdown();
}