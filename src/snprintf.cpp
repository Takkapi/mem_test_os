#include "snprintf.h"

// Function to convert integers to strings
static int itoa(int value, char* str, int base) {
    char* ptr = str, *ptr1 = str, temp_char;
    int tmp_value;
    if(base < 2 || base > 36) {
        *str = '\0';
        return 0;
    }

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while (value);
    
    // Apply negative sign
    if(tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        temp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = temp_char;
    }

    return tmp_value;
}

int snprintf(char* buffer, size_t size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    size_t i, j;
    for(i = 0, j = 0; format[i] != '\0' && j < size - 1; ++i) {
        if(format[i] == '%' && format[i + 1] == 'd') {
            int num = va_arg(args, int);
            char num_str[32];
            itoa(num, num_str, 10);
            
            for(int k = 0; num_str[k] != '\0' && j < size - 1; ++k, ++j) {
                buffer[j] = num_str[k];
            }
            ++i;
        } else {
            buffer[j++] = format[i];
        }
    }

    buffer[j] = '\0';
    va_end(args);
    return j;
}
