    OUTPUT_FORMAT("binary")    /* output flat binary */
    ENTRY(start)

    SECTIONS
    {
        . = 0;                 /* relocate to address 0 */

        .text ALIGN(0):
        {
            KEEP(*(.start.text))    /* include the .text section of start.o */
            *(.text)           /* include all other .text sections */
        }

        .data ALIGN(4):
        {
            *(.data)
        }

        .rodata ALIGN(4):
        {
            *(.rodata*)
        }
    }