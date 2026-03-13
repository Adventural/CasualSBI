/*
 * Simple standard I/O implementation
 */

#include <casualsbi.h>

void sbi_puts(const char *str)
{
    while (*str) {
        sbi_putc(*str++);
    }
}

static void print_dec(unsigned long n)
{
    char buf[32];
    int i = 0;
    
    if (n == 0) {
        sbi_putc('0');
        return;
    }
    
    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    
    while (i > 0) {
        sbi_putc(buf[--i]);
    }
}

static void print_hex(unsigned long n, int digits)
{
    const char hex_chars[] = "0123456789abcdef";
    char buf[16];
    int i = 0;
    
    if (digits == 0) {
        /* Auto-determine digits */
        digits = 1;
        unsigned long temp = n;
        while (temp >>= 4)
            digits++;
    }
    
    for (i = 0; i < digits; i++) {
        buf[digits - 1 - i] = hex_chars[n & 0xF];
        n >>= 4;
    }
    
    for (i = 0; i < digits; i++) {
        sbi_putc(buf[i]);
    }
}

void sbi_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt != '%') {
            sbi_putc(*fmt++);
            continue;
        }
        
        fmt++;
        
        switch (*fmt) {
        case 'c': {
            char c = va_arg(args, int);
            sbi_putc(c);
            break;
        }
        case 's': {
            const char *s = va_arg(args, const char *);
            if (s)
                sbi_puts(s);
            else
                sbi_puts("(null)");
            break;
        }
        case 'd':
        case 'i': {
            int n = va_arg(args, int);
            if (n < 0) {
                sbi_putc('-');
                n = -n;
            }
            print_dec((unsigned long)n);
            break;
        }
        case 'u': {
            unsigned int n = va_arg(args, unsigned int);
            print_dec(n);
            break;
        }
        case 'x':
        case 'X': {
            unsigned int n = va_arg(args, unsigned int);
            print_hex(n, 0);
            break;
        }
        case 'l': {
            fmt++;
            if (*fmt == 'x' || *fmt == 'X') {
                unsigned long n = va_arg(args, unsigned long);
                print_hex(n, 0);
            } else if (*fmt == 'd' || *fmt == 'u') {
                unsigned long n = va_arg(args, unsigned long);
                print_dec(n);
            } else {
                sbi_putc('%');
                sbi_putc('l');
                sbi_putc(*fmt);
            }
            break;
        }
        case 'p': {
            void *p = va_arg(args, void *);
            sbi_puts("0x");
            print_hex((unsigned long)p, sizeof(void *) * 2);
            break;
        }
        case '%':
            sbi_putc('%');
            break;
        default:
            sbi_putc('%');
            sbi_putc(*fmt);
            break;
        }
        
        if (*fmt)
            fmt++;
    }
    
    va_end(args);
}
