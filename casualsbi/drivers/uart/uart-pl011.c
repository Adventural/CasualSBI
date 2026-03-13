/*
 * ARM PL011 UART Driver for QEMU ARM64 Virt Platform
 */

#include <casualsbi.h>
#include <platform.h>

/* PL011 UART base address */
static volatile uint32_t *uart_base = (volatile uint32_t *)QEMU_VIRT_UART_BASE;

/* PL011 Register offsets (in 32-bit words) */
#define PL011_DR        0x00    /* Data Register */
#define PL011_RSR       0x01    /* Receive Status Register */
#define PL011_ECR       0x01    /* Error Clear Register */
#define PL011_FR        0x06    /* Flag Register */
#define PL011_IBRD      0x09    /* Integer Baud Rate Register */
#define PL011_FBRD      0x0A    /* Fractional Baud Rate Register */
#define PL011_LCRH      0x0B    /* Line Control Register */
#define PL011_CR        0x0C    /* Control Register */
#define PL011_IFLS      0x0D    /* Interrupt FIFO Level Select */
#define PL011_IMSC      0x0E    /* Interrupt Mask Set/Clear */
#define PL011_RIS       0x0F    /* Raw Interrupt Status */
#define PL011_MIS       0x10    /* Masked Interrupt Status */
#define PL011_ICR       0x11    /* Interrupt Clear Register */

/* PL011 FR bits */
#define PL011_FR_RXFE   0x10    /* Receive FIFO Empty */
#define PL011_FR_TXFF   0x20    /* Transmit FIFO Full */
#define PL011_FR_RXFF   0x40    /* Receive FIFO Full */
#define PL011_FR_TXFE   0x80    /* Transmit FIFO Empty */

/* PL011 CR bits */
#define PL011_CR_UARTEN 0x01    /* UART Enable */
#define PL011_CR_TXE    0x100   /* Transmit Enable */
#define PL011_CR_RXE    0x200   /* Receive Enable */

/* PL011 LCRH bits */
#define PL011_LCRH_FEN  0x10    /* FIFO Enable */
#define PL011_LCRH_WLEN_8BIT 0x60  /* 8-bit words */

static inline void pl011_write_reg(int reg, uint32_t val)
{
    uart_base[reg] = val;
}

static inline uint32_t pl011_read_reg(int reg)
{
    return uart_base[reg];
}

void uart_init(void)
{
    /* Disable UART */
    pl011_write_reg(PL011_CR, 0);
    
    /* Clear interrupts */
    pl011_write_reg(PL011_ICR, 0x7FF);
    
    /* Set baud rate - 115200 with 48MHz clock
     * IBRD = 48MHz / (16 * 115200) = 26
     * FBRD = 0.042 * 64 = 2.7 ≈ 3
     */
    pl011_write_reg(PL011_IBRD, 26);
    pl011_write_reg(PL011_FBRD, 3);
    
    /* 8 data bits, 1 stop bit, no parity, FIFO enabled */
    pl011_write_reg(PL011_LCRH, PL011_LCRH_WLEN_8BIT | PL011_LCRH_FEN);
    
    /* Enable UART, TX and RX */
    pl011_write_reg(PL011_CR, PL011_CR_UARTEN | PL011_CR_TXE | PL011_CR_RXE);
}

void uart_putc(char c)
{
    /* Wait for TX FIFO not full */
    while (pl011_read_reg(PL011_FR) & PL011_FR_TXFF)
        __asm__ volatile("nop");
    
    pl011_write_reg(PL011_DR, c);
}

char uart_getc(void)
{
    /* Wait for RX FIFO not empty */
    while (pl011_read_reg(PL011_FR) & PL011_FR_RXFE)
        __asm__ volatile("nop");
    
    return (char)pl011_read_reg(PL011_DR);
}

bool uart_avail(void)
{
    return !(pl011_read_reg(PL011_FR) & PL011_FR_RXFE);
}
