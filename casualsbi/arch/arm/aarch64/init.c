/*
 * ARM 64-bit Initialization
 * Placeholder for future ARM support
 */

#include <casualsbi.h>
#include <arch/arm/arch.h>

/* ARM initialization */
void arm_init(void)
{
    /* TODO: Implement ARM initialization */
    
    /* 
     * Steps needed:
     * 1. Setup exception vectors
     * 2. Configure MMU (optional for bare metal)
     * 3. Setup timers
     * 4. Enable interrupts
     */
}

/* Read ARM system timer (CNTVCT_EL0 or CNTPCT_EL0) */
uint64_t arm_read_time(void)
{
    uint64_t val;
    /* Use CNTPCT_EL0 (physical counter) for EL1 */
    __asm__ __volatile__("mrs %0, cntpct_el0" : "=r" (val));
    return val;
}

/* Exception handler (to be implemented) */
void arm_trap_handler(void)
{
    /* TODO: Implement exception handling */
    while (1) {
        __asm__ __volatile__("wfe");
    }
}
