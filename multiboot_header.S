.section .multiboot
.align 4
.long 0x1BADB002 # Multiboot header magic number
.long 0x00010003 # Multiboot header flags
.long -(0x1BADB002 + 0x00010003) # Checksum

.global _start

.section .text
.global _start
.type _start, @function
_start:
    # Placeholder for the actual start code
    jmp main_entry_point