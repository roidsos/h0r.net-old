#include "stdint.h"
struct MemorySegment{
    uint_64 length;
    MemorySegment* next;
    MemorySegment* previous;
    MemorySegment* nextfree;
    MemorySegment* previousfree;
    bool isfree;
};
struct AlignedMemorySegment{
    uint_64 MemorySegmentAddr : 63;
    bool IsAligned : 1;
};

void InitHeap(uint_64 heapstart ,uint_64 heaplength);
void* malloc(uint_64 size);
void* aligned_alloc(uint_64 alignment,uint_64 size);
void* calloc(uint_64 size);
void* realloc(void* old,uint_64 size);
void free(void* tofree);