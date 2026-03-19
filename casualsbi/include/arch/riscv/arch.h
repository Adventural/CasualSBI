/*
 * RISC-V Architecture Specific Definitions
 */

#ifndef _RISCV_ARCH_H_
#define _RISCV_ARCH_H_

#include <stdint.h>

/* RISC-V CSRs */
#define CSR_SSTATUS     0x100
#define CSR_SIE         0x104
#define CSR_STVEC       0x105
#define CSR_SCOUNTEREN  0x106
#define CSR_SSCRATCH    0x140
#define CSR_SEPC        0x141
#define CSR_SCAUSE      0x142
#define CSR_STVAL       0x143
#define CSR_SIP         0x144
#define CSR_SATP        0x180

#define CSR_TIME        0xC01
#define CSR_TIMEH       0xC81

/* MSTATUS */
#define MSTATUS_SIE     (1UL << 1)
#define MSTATUS_MIE     (1UL << 3)
#define MSTATUS_SPIE    (1UL << 5)
#define MSTATUS_MPIE    (1UL << 7)
#define MSTATUS_SPP     (1UL << 8)

/* MIE */
#define MIE_SSIE        (1UL << 1)
#define MIE_MSIE        (1UL << 3)
#define MIE_STIE        (1UL << 5)
#define MIE_MTIE        (1UL << 7)
#define MIE_SEIE        (1UL << 9)
#define MIE_MEIE        (1UL << 11)

/* SSTATUS fields */
#define SSTATUS_SIE     (1UL << 1)
#define SSTATUS_SPIE    (1UL << 5)
#define SSTATUS_SPP     (1UL << 8)

/* SIE/SIP fields */
#define SIP_SSIP        (1UL << 1)
#define SIP_STIP        (1UL << 5)
#define SIP_SEIP        (1UL << 9)

/* SBI Extension IDs */
#define SBI_EXT_0_1_CONSOLE_PUTCHAR 0x01
#define SBI_EXT_0_1_CONSOLE_GETCHAR 0x02
#define SBI_EXT_0_1_SHUTDOWN        0x08

#define SBI_EXT_BASE                0x10
#define SBI_EXT_TIME                0x54494D45
#define SBI_EXT_IPI                 0x735049
#define SBI_EXT_RFENCE              0x52464E43
#define SBI_EXT_HSM                 0x48534D

/* SBI Base Extension Function IDs */
#define SBI_EXT_BASE_GET_SPEC_VERSION   0
#define SBI_EXT_BASE_GET_IMP_ID         1
#define SBI_EXT_BASE_GET_IMP_VERSION    2
#define SBI_EXT_BASE_PROBE_EXT          3
#define SBI_EXT_BASE_GET_MVENDORID      4
#define SBI_EXT_BASE_GET_MARCHID        5
#define SBI_EXT_BASE_GET_MIMPID         6

/* SBI Return codes */
#define SBI_SUCCESS             0
#define SBI_ERR_FAILURE         -1
#define SBI_ERR_NOT_SUPPORTED   -2
#define SBI_ERR_INVALID_PARAM   -3
#define SBI_ERR_DENIED          -4
#define SBI_ERR_INVALID_ADDRESS -5

/* Inline assembly helpers */
#define csr_read(csr)                                           \
({                                                              \
    register uint64_t __v;                                      \
    __asm__ __volatile__ ("csrr %0, " #csr : "=r" (__v));      \
    __v;                                                        \
})

#define csr_write(csr, val)                                     \
({                                                              \
    uint64_t __v = (uint64_t)(val);                             \
    __asm__ __volatile__ ("csrw " #csr ", %0" : : "r" (__v));  \
})

#define csr_set(csr, val)                                       \
({                                                              \
    uint64_t __v = (uint64_t)(val);                             \
    __asm__ __volatile__ ("csrs " #csr ", %0" : : "r" (__v));  \
})

#define csr_clear(csr, val)                                     \
({                                                              \
    uint64_t __v = (uint64_t)(val);                             \
    __asm__ __volatile__ ("csrc " #csr ", %0" : : "r" (__v));  \
})

static inline void barrier(void)
{
    __asm__ __volatile__("" ::: "memory");
}

static inline void wfi(void)
{
    __asm__ __volatile__("wfi");
}

/* RISC-V specific initialization */
void riscv_init(void);
void riscv_trap_init(void);
void riscv_trap_handler_c(void);  /* Called from assembly trap handler */

/* Time functions */
uint64_t riscv_read_time(void);
void sbi_set_timer_modern(uint64_t stime_value);

/* SBI call for legacy console */
void sbi_legacy_console_putchar(int ch);
int sbi_legacy_console_getchar(void);
void sbi_legacy_shutdown(void);

/* Modern SBI call */
struct sbi_ret {
    long error;
    long value;
};

struct sbi_ret sbi_call(int ext, int fid, unsigned long arg0,
                        unsigned long arg1, unsigned long arg2,
                        unsigned long arg3, unsigned long arg4,
                        unsigned long arg5);

#endif /* _RISCV_ARCH_H_ */
