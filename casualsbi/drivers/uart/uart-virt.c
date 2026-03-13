/*
 * NS16550A UART Driver for QEMU RISC-V Virt Platform
 */

#include <casualsbi.h>

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

/* UART base address */
static volatile uint8_t *uart_base = (volatile uint8_t *)QEMU_VIRT_UART_BASE;

static inline void uart_write_reg(int reg, uint8_t val)
{
    uart_base[reg] = val;
}

static inline uint8_t uart_read_reg(int reg)
{
    return uart_base[reg];
}

void uart_init(void)
{
    /* Disable interrupts */
    uart_write_reg(UART_IER, 0x00);
    
    /* Enable DLAB to set baud rate */
    uart_write_reg(UART_LCR, UART_LCR_DLAB | UART_LCR_WLEN8);
    
    /* Set baud rate to 115200 (divider = 1 for QEMU) */
    /* Divisor Latch Low = 1, High = 0 */
    uart_write_reg(UART_DLL, 0x01);
    uart_write_reg(UART_DLM, 0x00);
    
    /* 8 data bits, 1 stop bit, no parity */
    uart_write_reg(UART_LCR, UART_LCR_WLEN8);
    
    /* Enable FIFO, clear them */
    uart_write_reg(UART_FCR, UART_FCR_FIFO_EN | UART_FCR_CLEAR_RX | UART_FCR_CLEAR_TX);
    
    /* Set DTR and RTS */
    uart_write_reg(UART_MCR, 0x03);
}

void uart_putc(char c)
{
    /* Wait for transmitter to be empty */
    while ((uart_read_reg(UART_LSR) & UART_LSR_THRE) == 0)
        __asm__ volatile("nop");
    
    uart_write_reg(UART_THR, c);
}

char uart_getc(void)
{
    /* Wait for data */
    while (!(uart_read_reg(UART_LSR) & UART_LSR_DR))
        ;
    
    return uart_read_reg(UART_RBR);
}

bool uart_avail(void)
{
    return (uart_read_reg(UART_LSR) & UART_LSR_DR) != 0;
}
