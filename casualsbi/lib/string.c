/*
 * String and memory utilities
 */

#include <casualsbi.h>

void *sbi_memset(void *s, int c, size_t n)
{
    unsigned char *p = s;
    
    while (n--) {
        *p++ = (unsigned char)c;
    }
    
    return s;
}

void *sbi_memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    
    while (n--) {
        *d++ = *s++;
    }
    
    return dest;
}

int sbi_memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *p1 = s1;
    const unsigned char *p2 = s2;
    
    while (n--) {
        if (*p1 != *p2)
            return (int)*p1 - (int)*p2;
        p1++;
        p2++;
    }
    
    return 0;
}

size_t sbi_strlen(const char *s)
{
    size_t len = 0;
    
    while (*s++) {
        len++;
    }
    
    return len;
}
