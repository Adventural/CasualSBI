/*
 * Platform-specific definitions
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdint.h>

struct platform_ops {
    void (*platform_call)(void);
};

/* Platform initialization */
void platform_early_init(void);
void platform_init(void);
void platform_final_init(void);

#endif /* _PLATFORM_H_ */
