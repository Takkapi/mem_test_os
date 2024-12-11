ENTRY(_start)
SECTIONS
{
    . = 0x100000; /* Load address at 1MB */
    .multiboot : { 
        *(.multiboot)
        PROVIDE(multiboot_start = .);
        *(.text)
    }
    .text : {
        PROVIDE(text_start = .);
        *(.text)
    }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(COMMON) *(.bss) }
    .end : { PROVIDE(_end = .); }
}
