/*
 * CasualSBI - Main entry point
 * Cross-architecture support
 */

#include <casualsbi.h>
#include <platform.h>

static struct sbi_interface sbi;

void sbi_putc(char c) 
{
    if (sbi.console.putc) {
        sbi.console.putc(c);
    }
}

char sbi_getc(void) 
{
    if (sbi.console.getc) {
        return sbi.console.getc();
    }
    return '\0';
}

void sbi_puts(const char *str)
{
    while (*str) {
        sbi_putc(*str++);
    }
}

void sbi_print_banner()
{
    sbi_printf("casual sbi start, version: %d.%d.%d\n", CASUALSBI_VERSION_MAJOR, 
        CASUALSBI_VERSION_MINOR, CASUALSBI_VERSION_PATCH);
}

void sbi_register_console(struct sbi_console_ops *ops)
{
    sbi.console.putc = ops->putc;
    sbi.console.getc = ops->getc;
}

void sbi_register_domain(struct sbi_console_ops *ops)
{
    sbi.console.putc = ops->putc;
    sbi.console.getc = ops->getc;
}

void sbi_init(void)
{
    /* early nitialize, eg: power, uart and etc */
    platform_early_init();

    //sbi_domain_init();

    sbi_print_banner();

    platform_init();

    // irq table
    // platform_irq_init(irq_base);
    // mm & int

    platform_final_init();

    // boot to next
    //sbi_detect_and_boot();

    int64_t i = 0;
    for (i = 0; i < 100000000000; i++) {
        if (i % 100000000 == 0) {
            sbi_printf("count: %ld\n", i);
        }
    }

    *((int32_t*)0) = 0;
    
}
