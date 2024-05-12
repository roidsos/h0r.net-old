#ifndef __HEAP_H__
#define __HEAP_H__

#include <types/stdtypes.h>

typedef struct memseg{
    uint64_t length;
    struct memseg* next;
    struct memseg* previous;
    struct memseg* nextfree;
    struct memseg* previousfree;
    bool isfree;
} mem_seg;

void heap_init(uint64_t heap_start ,size_t heap_size);
void* malloc(size_t size);
void* calloc(size_t size);
void* realloc(void* old,size_t size);
void* realloc_plus(void* old,size_t size,size_t oldsize);
void free(void* tofree);
#endif // __HEAP_H__
