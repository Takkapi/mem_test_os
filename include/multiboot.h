#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

// Multiboot header magic number
#define MULTIBOOT_HEADER_MAGIC 0x1BADB002

// Multiboot header flags
#define MULTIBOOT_HEADER_FLAGS 0x00010003

// Checksum of the multiboot header
#define MULTIBOOT_HEADER_CHECKSUM (uint32_t)(-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS))

#endif