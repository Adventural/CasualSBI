/*
 * Linker script for CasualSBI on QEMU Virt Platform
 */

OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    ram (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS
{
    . = ORIGIN(ram);
    
    /* Text section */
    .text : ALIGN(8) {
        _text_start = .;
        *(.text.boot)
        *(.text .text.*)
        . = ALIGN(8);
        _text_end = .;
    } > ram
    
    /* Read-only data */
    .rodata : ALIGN(8) {
        _rodata_start = .;
        *(.rodata .rodata.*)
        . = ALIGN(8);
        _rodata_end = .;
    } > ram
    
    /* Initialized data */
    .data : ALIGN(8) {
        _data_start = .;
        *(.data .data.*)
        . = ALIGN(8);
        _data_end = .;
    } > ram
    
    /* Uninitialized data / BSS */
    .bss (NOLOAD) : ALIGN(16) {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(16);
        _bss_end = .;
    } > ram
    
    _end = .;
    
    /* Discard unused sections */
    /DISCARD/ : {
        *(.comment)
        *(.note)
        *(.note.*)
        *(.eh_frame)
        *(.gnu.*)
    }
}
