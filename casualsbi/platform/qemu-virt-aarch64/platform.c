/*
 * QEMU Virt Platform Support
 * Cross-architecture implementation
 */

#include <casualsbi.h>
#include <platform.h>
#include <arch/arm/arch.h>

#define QEMU_VIRT_DRAM_BASE     0x40000000UL
#define QEMU_VIRT_UART_BASE     0x9000000UL   /* PL011 */

/* Console operations using UART directly */
static void console_init_direct(void)
{
    uart_init();
}

static void console_putc_direct(char c)
{
    uart_putc(c);
}

static char console_getc_direct(void)
{
    return uart_getc();
}

static bool console_avail_direct(void)
{
    return uart_avail();
}

static struct sbi_console_ops qemu_console_ops;
static struct sbi_timer_ops qemu_timer_ops;
static struct sbi_platform_ops qemu_platform_ops;

/* Platform early initialization - called before C runtime is ready */
void platform_early_init(void)
{
    /* Just initialize UART for early output */
    uart_init();
}

/* Architecture-specific timer functions */
#if defined(ARCH_RISCV)

/* Timer operations for RISC-V */
static void riscv_timer_init(void)
{
    /* Timer initialized by architecture code */
}

static uint64_t riscv_timer_get_time(void)
{
    return csr_read(time);
}

static void riscv_timer_set_timer(uint64_t stime)
{
    /* Use SBI timer call if available, otherwise direct CLINT access */
    /* For now, simplified version */
    (void)stime;
}

#elif defined(ARCH_ARM)

/* Timer operations for ARM */
static void arm_timer_init(void)
{
    /* ARM timer initialization would go here */
}

static uint64_t arm_timer_get_time(void)
{
    return arm_read_time();
}

static void arm_timer_set_timer(uint64_t stime)
{
    (void)stime;
}

#endif

/* Platform main initialization */
void platform_init(void)
{
    /* Use direct UART access for bare metal */
    qemu_console_ops.init = console_init_direct;
    qemu_console_ops.putc = console_putc_direct;
    qemu_console_ops.getc = console_getc_direct;
    qemu_console_ops.avail = console_avail_direct;
    
    /* Timer operations - architecture specific */
#if defined(ARCH_RISCV)
    qemu_timer_ops.init = riscv_timer_init;
    qemu_timer_ops.get_time = riscv_timer_get_time;
    qemu_timer_ops.set_timer = riscv_timer_set_timer;
#elif defined(ARCH_ARM)
    qemu_timer_ops.init = arm_timer_init;
    qemu_timer_ops.get_time = arm_timer_get_time;
    qemu_timer_ops.set_timer = arm_timer_set_timer;
#endif
    
    /* Platform operations */
    qemu_platform_ops.early_init = platform_early_init;
    qemu_platform_ops.init = platform_init;
    qemu_platform_ops.shutdown = platform_shutdown;
    qemu_platform_ops.reboot = platform_reboot;
    
    /* Call the console init */
    if (qemu_console_ops.init)
        qemu_console_ops.init();
}

/* Platform-specific console operations */
const struct sbi_console_ops *platform_get_console_ops(void)
{
    return &qemu_console_ops;
}

const struct sbi_timer_ops *platform_get_timer_ops(void)
{
    return &qemu_timer_ops;
}

const struct sbi_platform_ops *platform_get_platform_ops(void)
{
    return &qemu_platform_ops;
}

void platform_shutdown(void)
{
#if defined(ARCH_RISCV)
    /* On RISC-V, try to use SBI shutdown */
    /* Note: This won't work without an SBI implementation */
    /* For QEMU, we can use the test finisher device */
    volatile uint32_t *test_finisher = (volatile uint32_t *)0x100000;
    *test_finisher = 0x5555;  /* QEMU test finisher: exit code 0 */
#elif defined(ARCH_ARM)
    /* On ARM, use QEMU's PSCI or similar mechanism */
    /* For now, just hang */
#endif
    
    /* If still running, spin */
    while (1) {
        __asm__ __volatile__("wfi");
    }
}

void platform_reboot(void)
{
#if defined(ARCH_RISCV)
    /* No standard RISC-V reboot mechanism for bare metal */
#elif defined(ARCH_ARM)
    /* Could use PSCI on ARM */
#endif
    
    /* If still running, spin */
    while (1) {
        __asm__ __volatile__("wfi");
    }
}

void platform_init(void)
{


    platform_register_ops()
}
