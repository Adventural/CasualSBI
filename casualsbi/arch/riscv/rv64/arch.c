/*
 * RISC-V 64-bit Initialization
 */

#include <casualsbi.h>
#include <arch/riscv/arch.h>
#include <platform.h>

/* Forward declarations from entry.S */
extern unsigned long _fdt_addr;

/* Assembly trap entry defined in trap.S */
extern void m_trap_entry(void);

struct arch_regs {
    uint64_t regs[32];   // x0 - x31
    uint64_t mstatus; 
    uint64_t mepc;
    uint64_t mtval;
    uint64_t mcause;
};

void dump_regs(struct arch_regs *mregs) 
{
    const char *reg_names[] = {
        "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
        "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
        "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
        "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
    };

    sbi_printf("\n--- [ CPU Register Dump ] ---\n");
    for (int i = 0; i < 32; i++) {
        sbi_printf("%s: 0x%lx\n", reg_names[i], mregs->regs[i]);
        // if ((i + 1) % 2 == 0) sbi_printf("\n");
    }

    sbi_printf("------------------------------\n");
    sbi_printf("mepc   : 0x%lx\n", mregs->mepc);
    sbi_printf("mcause : 0x%lx\n", mregs->mcause);
    sbi_printf("mtval  : 0x%lx\n", mregs->mtval);
    sbi_printf("mstatus: 0x%lx\n", mregs->mstatus);
    sbi_printf("------------------------------\n");
}

/* Trap handler - defined in trap.S, called from assembly */
void m_trap_handler(struct arch_regs *mregs)
{
    /* just dump regs now */
    dump_regs(mregs);

    wfi();
}

void trap_init(void)
{
    /* set trap vector */
    csr_write(mtvec, (uintptr_t)m_trap_entry);
    
    /* enable interrupts in m-mode */
    csr_set(mstatus, MSTATUS_MIE);
}

void arch_init(void)
{
    trap_init();
}
