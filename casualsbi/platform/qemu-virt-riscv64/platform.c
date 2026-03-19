/*
 * QEMU Virt Platform Support
 * Cross-architecture implementation
 */

#include <casualsbi.h>
#include <platform.h>
#include <arch/riscv/arch.h>
#include <drivers/uart/uart.h>
#include <drivers/plic/plic.h>

#define QEMU_VIRT_DRAM_BASE     0x80000000UL
#define QEMU_VIRT_DRAM_SIZE     0x8000000UL   /* 128MB */
#define QEMU_VIRT_UART_BASE     0x10000000UL  /* NS16550A */
#define QEMU_VIRT_PLIC_BASE     0x0C000000UL

typedef struct {
    plic_t plic;
} qemu_virt_t;

static qemu_virt_t qemu_virt;

void platform_early_init(void)
{
    uart_init((uint8_t*)QEMU_VIRT_UART_BASE);
}

void platform_init(void)
{
    plic_t *plic = &qemu_virt.plic;

    plic->base = QEMU_VIRT_PLIC_BASE;
    plic_init(plic);

    plic_set_priority(plic, 10, 1);
    plic_irq_enable(plic, 0, 10);
    plic_set_threshold(plic, 0, 0);

    csr_set(mie, MIE_MEIE);
}

void platform_final_init(void)
{

}
