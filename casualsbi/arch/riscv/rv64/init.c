/*
 * RISC-V 64-bit Initialization
 */

#include <casualsbi.h>
#include <arch/riscv/arch.h>
#include <platform.h>

/* Forward declarations from entry.S */
extern unsigned long _fdt_addr;

/* SBI call implementation for RISC-V */
struct sbi_ret sbi_call(int ext, int fid, unsigned long arg0,
                        unsigned long arg1, unsigned long arg2,
                        unsigned long arg3, unsigned long arg4,
                        unsigned long arg5)
{
    struct sbi_ret ret;
    
    register unsigned long a0 __asm__("a0") = arg0;
    register unsigned long a1 __asm__("a1") = arg1;
    register unsigned long a2 __asm__("a2") = arg2;
    register unsigned long a3 __asm__("a3") = arg3;
    register unsigned long a4 __asm__("a4") = arg4;
    register unsigned long a5 __asm__("a5") = arg5;
    register unsigned long a6 __asm__("a6") = fid;
    register unsigned long a7 __asm__("a7") = ext;
    
    __asm__ __volatile__ (
        "ecall"
        : "+r" (a0), "+r" (a1)
        : "r" (a2), "r" (a3), "r" (a4), "r" (a5),
          "r" (a6), "r" (a7)
        : "memory"
    );
    
    ret.error = a0;
    ret.value = a1;
    return ret;
}

/* Legacy SBI console functions */
void sbi_legacy_console_putchar(int ch)
{
    sbi_call(SBI_EXT_0_1_CONSOLE_PUTCHAR, 0, ch, 0, 0, 0, 0, 0);
}

int sbi_legacy_console_getchar(void)
{
    struct sbi_ret ret = sbi_call(SBI_EXT_0_1_CONSOLE_GETCHAR, 0, 0, 0, 0, 0, 0, 0);
    return (int)ret.value;
}

void sbi_legacy_shutdown(void)
{
    sbi_call(SBI_EXT_0_1_SHUTDOWN, 0, 0, 0, 0, 0, 0, 0);
}

/* Modern SBI timer functions */
void sbi_set_timer_modern(uint64_t stime_value)
{
    sbi_call(SBI_EXT_TIME, 0, stime_value, 0, 0, 0, 0, 0);
}

/* Read time from CSR */
uint64_t riscv_read_time(void)
{
#if __riscv_xlen == 64
    return csr_read(time);
#else
    uint32_t lo, hi, tmp;
    __asm__ __volatile__ (
        "1: rdtimeh %0\n"
        "   rdtime %1\n"
        "   rdtimeh %2\n"
        "   bne %0, %2, 1b"
        : "=r" (hi), "=r" (lo), "=r" (tmp)
    );
    return ((uint64_t)hi << 32) | lo;
#endif
}

/* RISC-V initialization */
void riscv_init(void)
{
    /* Initialize trap handling */
    riscv_trap_init();
    
    /* Enable supervisor timer interrupt */
    csr_set(sie, SIP_STIP);
    
    /* Enable access to time CSR from user/supervisor mode */
    csr_write(scounteren, 0x2);
}

/* Trap handler - defined in trap.S, called from assembly */
void riscv_trap_handler_c(void)
{
    uint64_t cause = csr_read(scause);
    (void)cause;  /* Suppress unused warning for now */
    
    /* Check if it's an interrupt */
    if (cause & (1UL << 63)) {
        cause &= ~(1UL << 63);
        
        switch (cause) {
        case 5: /* Supervisor timer interrupt */
            /* Clear the interrupt */
            csr_clear(sip, SIP_STIP);
            /* Timer handler would go here */
            break;
        default:
            sbi_printf("Unknown interrupt: %lu\n", cause);
            break;
        }
    } else {
        sbi_printf("Exception: cause=%lu, epc=0x%lx\n", 
                   cause, csr_read(sepc));
        sbi_shutdown();
    }
}

/* Assembly trap entry defined in trap.S */
extern void riscv_trap_entry(void);

void riscv_trap_init(void)
{
    /* Set trap vector */
    csr_write(stvec, (uintptr_t)riscv_trap_entry);
    
    /* Enable supervisor interrupts */
    csr_set(sstatus, SSTATUS_SIE);
}
