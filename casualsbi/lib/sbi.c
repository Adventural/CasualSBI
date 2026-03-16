/*
 * CasualSBI - Main entry point
 * Cross-architecture support
 */

#include <casualsbi.h>
#include <platform.h>

void sbi_print_banner()
{
    sbi_printf();
}

void sbi_init(void)
{
    /* early nitialize, eg: power, uart and etc */
    platform_early_init();

    sbi_domain_init(void);

    sbi_print_banner();

    // irq table
    platform_irq_init(irq_base);
    // mm & int

    platform_final_init();

    // boot to next
    sbi_detect_and_boot();
}
