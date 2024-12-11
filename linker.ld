ENTRY(_start)
SECTIONS
{
    . = 1M; /* Load address at 1MB */
    .multiboot : { *(.multiboot) }
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(COMMON) *(.bss) }
}
