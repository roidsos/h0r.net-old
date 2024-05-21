#ifndef __HEAP_H__
#define __HEAP_H__

#include <libk/stddef.h>

void* malloc(usize size);
void* calloc(usize count, usize size);
void* realloc(void* old,usize size);
void* realloc_plus(void* old,usize size,usize oldsize);
void free(void* tofree);
#endif // __HEAP_H__
