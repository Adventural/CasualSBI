/*
 * ARM Architecture Specific Definitions
 * Placeholder for future ARM support
 */

#ifndef _ARM_ARCH_H_
#define _ARM_ARCH_H_

#include <stdint.h>

/* ARM processor modes */
#define ARM_MODE_USR    0x10
#define ARM_MODE_FIQ    0x11
#define ARM_MODE_IRQ    0x12
#define ARM_MODE_SVC    0x13
#define ARM_MODE_MON    0x16
#define ARM_MODE_ABT    0x17
#define ARM_MODE_UND    0x1B
#define ARM_MODE_SYS    0x1F

/* ARMv7-A/V8-A Exception Vector offsets */
#define ARM_VECTOR_RST      0x00
#define ARM_VECTOR_UND      0x04
#define ARM_VECTOR_SWI      0x08
#define ARM_VECTOR_IABT     0x0C
#define ARM_VECTOR_DABT     0x10
#define ARM_VECTOR_UNUSED   0x14
#define ARM_VECTOR_IRQ      0x18
#define ARM_VECTOR_FIQ      0x1C

/* ARMv8-A AArch64 Exception Class */
#define ARM_EC_UNKNOWN      0x00
#define ARM_EC_WFI_WFE      0x01
#define ARM_EC_MCR_MRC_CP15 0x03
#define ARM_EC_MCRR_MRRC_CP15 0x04
#define ARM_EC_MCR_MRC_CP14 0x05
#define ARM_EC_LDC_STC_CP14 0x06
#define ARM_EC_SIMD_FP      0x07
#define ARM_EC_MCR_MRC_CP10 0x08
#define ARM_EC_MRRC_CP14    0x0C
#define ARM_EC_ILLEGAL      0x0E
#define ARM_EC_SVC_AARCH32  0x11
#define ARM_EC_HVC_AARCH32  0x12
#define ARM_EC_SMC_AARCH32  0x13
#define ARM_EC_SVC_AARCH64  0x15
#define ARM_EC_HVC_AARCH64  0x16
#define ARM_EC_SMC_AARCH64  0x17
#define ARM_EC_SYS_REG      0x18
#define ARM_EC_SVE          0x19
#define ARM_EC_IABT_EL0     0x20
#define ARM_EC_IABT_EL1     0x21
#define ARM_EC_PC_ALIGN     0x22
#define ARM_EC_DABT_EL0     0x24
#define ARM_EC_DABT_EL1     0x25
#define ARM_EC_SP_ALIGN     0x26
#define ARM_EC_FP_EXC_32    0x28
#define ARM_EC_FP_EXC_64    0x2C
#define ARM_EC_SError       0x2F
#define ARM_EC_BRKPT_EL0    0x30
#define ARM_EC_BRKPT_EL1    0x31
#define ARM_EC_SS_STEP_EL0  0x32
#define ARM_EC_SS_STEP_EL1  0x33
#define ARM_EC_WATCHPT_EL0  0x34
#define ARM_EC_WATCHPT_EL1  0x35
#define ARM_EC_BKPT_32      0x38
#define ARM_EC_VECTOR_32    0x3A

/* ARM system registers (AArch64) */
#define ARM_REG_CNTPCT_EL0  "cntpct_el0"
#define ARM_REG_CNTP_CTL_EL0 "cntp_ctl_el0"
#define ARM_REG_CNTP_CVAL_EL0 "cntp_cval_el0"
#define ARM_REG_CNTVCT_EL0  "cntvct_el0"
#define ARM_REG_CURRENTEL   "CurrentEL"
#define ARM_REG_DAIF        "DAIF"
#define ARM_REG_SPSR_EL1    "spsr_el1"
#define ARM_REG_ELR_EL1     "elr_el1"
#define ARM_REG_SP_EL0      "sp_el0"
#define ARM_REG_SP_EL1      "sp_el1"
#define ARM_REG_VBAR_EL1    "vbar_el1"
#define ARM_REG_MAIR_EL1    "mair_el1"
#define ARM_REG_TCR_EL1     "tcr_el1"
#define ARM_REG_TTBR0_EL1   "ttbr0_el1"
#define ARM_REG_TTBR1_EL1   "ttbr1_el1"
#define ARM_REG_SCTLR_EL1   "sctlr_el1"

/* Inline assembly helpers for AArch64 */
#define arm_read_sysreg(reg)                                    \
({                                                              \
    uint64_t __val;                                             \
    __asm__ __volatile__("mrs %0, " #reg : "=r" (__val));       \
    __val;                                                      \
})

#define arm_write_sysreg(reg, val)                              \
({                                                              \
    uint64_t __val = (uint64_t)(val);                           \
    __asm__ __volatile__("msr " #reg ", %0" : : "r" (__val));   \
})

/* Barrier instructions */
#define arm_dsb()   __asm__ __volatile__("dsb sy" ::: "memory")
#define arm_dmb()   __asm__ __volatile__("dmb sy" ::: "memory")
#define arm_isb()   __asm__ __volatile__("isb" ::: "memory")

/* ARM initialization functions (to be implemented) */
void arm_init(void);
void arm_trap_init(void);
uint64_t arm_read_time(void);

#endif /* _ARM_ARCH_H_ */
