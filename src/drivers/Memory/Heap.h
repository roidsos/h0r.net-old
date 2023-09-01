#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdint.h>
#include <stdbool.h>

struct MemorySegment{
    uint64_t length;
    struct MemorySegment* next;
    struct MemorySegment* previous;
    struct MemorySegment* nextfree;
    struct MemorySegment* previousfree;
    bool isfree;
};
struct AlignedMemorySegment{
    uint64_t MemorySegmentAddr : 63;
    bool IsAligned : 1;
};

void InitHeap(uint64_t heapstart ,uint64_t heaplength);
void* malloc(uint64_t size);
void* aligned_alloc(uint64_t alignment,uint64_t size);
void* calloc(uint64_t size);
void* realloc(void* old,uint64_t size);
void free(void* tofree);
#endif // __HEAP_H__