/*
 * Platform-specific definitions
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdint.h>

struct platform_ops {
    void (*platform_call)(void);
};

void platform_init(void);

/* Platform initialization */
void platform_early_init(void);
void platform_init(void);
void platform_shutdown(void);
void platform_reboot(void);

/* Platform UART functions */
// void uart_init(void);
// void uart_putc(char c);
// char uart_getc(void);
// bool uart_avail(void);

#endif /* _PLATFORM_H_ */
