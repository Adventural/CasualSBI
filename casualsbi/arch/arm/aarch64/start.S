/*
 * ARM 64-bit (AArch64) Boot Code
 * Entry point for CasualSBI on ARM64
 */

.section .text.boot
.global _start

/* QEMU Virt UART base for early debug */
.equ UART_BASE, 0x9000000

_start:
    /* 
     * Entry point for ARM64 processor
     * x0 = device tree address (if passed by bootloader)
     * Running in EL2 (hypervisor) or EL3 (secure monitor)
     */
    
    /* Save device tree address */
    ldr x1, =_fdt_addr
    str x0, [x1]
    
    /* Setup stack pointer */
    ldr x0, =_stack_top
    mov sp, x0
    
    /* Clear BSS */
    ldr x0, =_bss_start
    ldr x1, =_bss_end
    mov x2, #0
_clear_bss:
    cmp x0, x1
    b.ge _bss_done
    str x2, [x0], #8
    b _clear_bss
_bss_done:
    
    /* Jump to C code */
    bl sbi_init
    
    /* Should never return */
_park:
    wfe
    b _park

.section .rodata
.global _fdt_addr
_fdt_addr:
    .quad 0

.section .bss
.align 16
_stack_bottom:
    .space 16384    /* 16KB stack */
_stack_top:
