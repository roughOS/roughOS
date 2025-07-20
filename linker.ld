ENTRY(_start)

SECTIONS
{
    . = 1M;

    .multiboot ALIGN(8) :
    {
        *(.multiboot)
    }

    .text ALIGN(4) :
    {
        *(.text)
    }

    .data ALIGN(4) :
    {
        *(.data)
    }

    .bss ALIGN(4) :
    {
        *(.bss)
        *(COMMON)
    }

    .rodata ALIGN(4) :
    {
        *(.rodata)
    }
}