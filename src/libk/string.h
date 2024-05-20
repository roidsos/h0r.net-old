#ifndef __STRING_H__
#define __STRING_H__

#include <libk/stdint.h>
#include <libk/stddef.h>

void *memchr(const void *, int, usize);
int memcmp(const void *, const void *, usize);
void *memcpy(void *, const void *, usize);
void *memmove(void *, const void *, usize);
void *memset(void *, int, usize);
char *strcat(char *, const char *);
int strcmp(const char *, const char *);
char *strcpy(char *, const char *);
usize strcspn(const char*, const char *);
usize strlen(const char *);
char *strncat(char *, const char *, usize);
int strncmp(const char *, const char *, usize);
char *strncpy(char *, const char *, usize);
usize strspn(const char *, const char *);
char *strtok(char *, const char *);
char *strchr(const char *s, int c);

#endif // __STRING_H__