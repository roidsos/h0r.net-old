#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdint.h>
typedef struct {
    uint64_t length;
    MemorySegment* next;
    MemorySegment* previous;
    MemorySegment* nextfree;
    MemorySegment* previousfree;
    bool isfree;
}MemorySegment;
typedef struct {
    uint64_t MemorySegmentAddr : 63;
    bool IsAligned : 1;
}AlignedMemorySegment;

void InitHeap(uint64_t heapstart ,uint64_t heaplength);
void* malloc(uint64_t size);
void* aligned_alloc(uint64_t alignment,uint64_t size);
void* calloc(uint64_t size);
void* realloc(void* old,uint64_t size);
void free(void* tofree);
#endif // __HEAP_H__