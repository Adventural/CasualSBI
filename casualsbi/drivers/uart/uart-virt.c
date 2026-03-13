/*
 * NS16550A UART Driver for QEMU RISC-V Virt Platform
 */

#include <casualsbi.h>
#include <platform.h>

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
