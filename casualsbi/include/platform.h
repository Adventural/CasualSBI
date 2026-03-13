/*
 * Platform-specific definitions
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdint.h>

/* QEMU Virt Platform Memory Map - Architecture Specific */
#if defined(ARCH_RISCV)
    #define QEMU_VIRT_DRAM_BASE     0x80000000UL
    #define QEMU_VIRT_UART_BASE     0x10000000UL  /* NS16550A */
    
    /* NS16550A UART Registers */
    #define UART_THR    0x00    /* Transmitter Holding Register */
    #define UART_RBR    0x00    /* Receiver Buffer Register */
    #define UART_DLL    0x00    /* Divisor Latch Low */
    #define UART_IER    0x01    /* Interrupt Enable Register */
    #define UART_DLM    0x01    /* Divisor Latch High */
    #define UART_IIR    0x02    /* Interrupt Identity Register */
    #define UART_FCR    0x02    /* FIFO Control Register */
    #define UART_LCR    0x03    /* Line Control Register */
    #define UART_MCR    0x04    /* Modem Control Register */
    #define UART_LSR    0x05    /* Line Status Register */
    #define UART_MSR    0x06    /* Modem Status Register */
    #define UART_SCR    0x07    /* Scratch Register */

    /* UART LSR bits */
    #define UART_LSR_DR     0x01    /* Data Ready */
    #define UART_LSR_THRE   0x20    /* Transmitter Holding Register Empty */
    #define UART_LSR_TEMT   0x40    /* Transmitter Empty */

    /* UART FCR bits */
    #define UART_FCR_FIFO_EN    0x01
    #define UART_FCR_CLEAR_RX   0x02
    #define UART_FCR_CLEAR_TX   0x04

    /* UART LCR bits */
    #define UART_LCR_WLEN8      0x03    /* 8-bit words */
    #define UART_LCR_DLAB       0x80    /* Divisor Latch Access */

#elif defined(ARCH_ARM)
    #define QEMU_VIRT_DRAM_BASE     0x40000000UL
    #define QEMU_VIRT_UART_BASE     0x9000000UL   /* PL011 */
#endif

#define QEMU_VIRT_DRAM_SIZE     0x8000000UL   /* 128MB */

/* Platform initialization */
void platform_early_init(void);
void platform_init(void);
void platform_shutdown(void);
void platform_reboot(void);

/* Platform UART functions */
void uart_init(void);
void uart_putc(char c);
char uart_getc(void);
bool uart_avail(void);

#endif /* _PLATFORM_H_ */
