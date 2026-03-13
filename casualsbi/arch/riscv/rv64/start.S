/*
 * RISC-V 64-bit Boot Code
 * Entry point for CasualSBI
 */

.section .text.boot
.global _start

/* QEMU Virt UART address */
.equ UART_BASE, 0x10000000
.equ UART_THR, 0x00
.equ UART_LSR, 0x05
.equ UART_LSR_THRE, 0x20

_start:
    /* 
     * The hartid is in a0, device tree address is in a1
     * Only hart 0 continues, others park themselves
     */
    bnez a0, _park

    /* Save FDT address */
    la t0, _fdt_addr
    sd a1, (t0)

    /* Setup stack pointer */
    la sp, _stack_top

    /* Clear BSS */
    la t0, _bss_start
    la t1, _bss_end
_clear_bss:
    bge t0, t1, _bss_done
    sd zero, (t0)
    addi t0, t0, 8
    j _clear_bss
_bss_done:

    /* Jump to C code */
    call sbi_init

    /* Should never return */
_park:
    wfi
    j _park

.section .rodata
.global _fdt_addr
_fdt_addr:
    .dword 0

.section .bss
.align 16
_stack_bottom:
    .space 16384    /* 16KB stack */
_stack_top:
