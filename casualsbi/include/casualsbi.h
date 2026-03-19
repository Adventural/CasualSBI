/*
 * CasualSBI - Hardware Abstraction Layer
 * Main header file
 */

#ifndef _CASUALSBI_H_
#define _CASUALSBI_H_

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

/* version */
#define CASUALSBI_VERSION_MAJOR 0
#define CASUALSBI_VERSION_MINOR 1
#define CASUALSBI_VERSION_PATCH 0

/* result codes */
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

/* sbi console operations */
struct sbi_console_ops {
    void (*putc)(char c);
    char (*getc)(void);
};

/* Main SBI interface structure */
struct sbi_interface {
    struct sbi_console_ops console;
};

/* sbi interface */
void sbi_init(void);
void sbi_register_console(struct sbi_console_ops *ops);

void sbi_putc(char c);
char sbi_getc(void);
void sbi_puts(const char *str);
void sbi_printf(const char *fmt, ...);

void *sbi_memset(void *s, int c, size_t n);
void *sbi_memcpy(void *dest, const void *src, size_t n);
int sbi_memcmp(const void *s1, const void *s2, size_t n);
size_t sbi_strlen(const char *s);

#endif /* _CASUALSBI_H_ */
