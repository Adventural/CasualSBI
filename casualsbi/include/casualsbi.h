/*
 * CasualSBI - Hardware Abstraction Layer
 * Main header file
 */

#ifndef _CASUALSBI_H_
#define _CASUALSBI_H_

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

/* Version information */
#define CASUALSBI_VERSION_MAJOR 0
#define CASUALSBI_VERSION_MINOR 1
#define CASUALSBI_VERSION_PATCH 0

/* Result codes */
typedef enum {
    SBI_OK = 0,
    SBI_ERR_FAILED = -1,
    SBI_ERR_NOT_SUPPORTED = -2,
    SBI_ERR_INVALID_PARAM = -3,
    SBI_ERR_DENIED = -4,
    SBI_ERR_INVALID_ADDRESS = -5,
    SBI_ERR_ALREADY_AVAILABLE = -6,
    SBI_ERR_ALREADY_STARTED = -7,
    SBI_ERR_ALREADY_STOPPED = -8,
} sbi_result_t;

/* Console operations */
struct sbi_console_ops {
    void (*init)(void);
    void (*putc)(char c);
    char (*getc)(void);
    bool (*avail)(void);
};

/* Timer operations */
struct sbi_timer_ops {
    void (*init)(void);
    uint64_t (*get_time)(void);
    void (*set_timer)(uint64_t stime_value);
};

/* Interrupt operations */
struct sbi_irq_ops {
    void (*init)(void);
    void (*enable)(void);
    void (*disable)(void);
    void (*register_handler)(int irq, void (*handler)(void));
};

/* Memory management operations */
struct sbi_mem_ops {
    void (*init)(void);
    void *(*alloc)(size_t size);
    void (*free)(void *ptr);
};

/* Platform operations */
struct sbi_platform_ops {
    void (*early_init)(void);
    void (*init)(void);
    void (*shutdown)(void);
    void (*reboot)(void);
};

/* Main SBI interface structure */
struct sbi_interface {
    const struct sbi_console_ops *console;
    const struct sbi_timer_ops *timer;
    const struct sbi_irq_ops *irq;
    const struct sbi_mem_ops *mem;
    const struct sbi_platform_ops *platform;
};

/* Global SBI interface */
extern const struct sbi_interface *sbi;

/* System initialization */
void sbi_init(void);
void sbi_main(void);

/* Console functions */
static inline void sbi_putc(char c) {
    if (sbi && sbi->console && sbi->console->putc)
        sbi->console->putc(c);
}

static inline char sbi_getc(void) {
    if (sbi && sbi->console && sbi->console->getc)
        return sbi->console->getc();
    return '\0';
}

static inline bool sbi_console_avail(void) {
    if (sbi && sbi->console && sbi->console->avail)
        return sbi->console->avail();
    return false;
}

void sbi_puts(const char *str);
void sbi_printf(const char *fmt, ...);

/* Timer functions */
static inline uint64_t sbi_get_time(void) {
    if (sbi && sbi->timer && sbi->timer->get_time)
        return sbi->timer->get_time();
    return 0;
}

static inline void sbi_set_timer(uint64_t stime) {
    if (sbi && sbi->timer && sbi->timer->set_timer)
        sbi->timer->set_timer(stime);
}

/* Platform functions */
static inline void sbi_shutdown(void) {
    if (sbi && sbi->platform && sbi->platform->shutdown)
        sbi->platform->shutdown();
    while (1);
}

static inline void sbi_reboot(void) {
    if (sbi && sbi->platform && sbi->platform->reboot)
        sbi->platform->reboot();
    while (1);
}

/* Utility functions */
void *sbi_memset(void *s, int c, size_t n);
void *sbi_memcpy(void *dest, const void *src, size_t n);
int sbi_memcmp(const void *s1, const void *s2, size_t n);
size_t sbi_strlen(const char *s);

#endif /* _CASUALSBI_H_ */
