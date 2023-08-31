#ifndef __ESSENTIAL_H__
#define __ESSENTIAL_H__
#include <stdint.h>
#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
// Halt and catch fire function.
void hcf(void);
#endif // __ESSENTIAL_H__