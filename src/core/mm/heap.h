#ifndef __HEAP_H__
#define __HEAP_H__

#include <types/stdtypes.h>

void heap_init();
void* malloc(size_t size);
void* calloc(size_t count, size_t size);
void* realloc(void* old,size_t size);
void* realloc_plus(void* old,size_t size,size_t oldsize);
void free(void* tofree);
#endif // __HEAP_H__
