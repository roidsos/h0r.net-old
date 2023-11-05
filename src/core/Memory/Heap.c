#include "Heap.h"
#include "PFA.h"
#include <klibc/memory.h>

void *malloc(size_t size) {
    char *PP = (char *)request_pages((size / 4096) + 1);
    *((size_t *)PP) = size;
    PP += sizeof(size_t); // Note: this value is architecture dependent
    return (void *)PP;
}

void free(void *tofree) {
    char *PP = (char *)tofree;
    PP -= sizeof(size_t); // Note: this value is architecture dependent
    size_t size = *((size_t *)PP);
    free_pages(PP, (size / 4096) + 1);
}
void *calloc(size_t size) {
    void *mallocVal = malloc(size);
    memset(mallocVal, 0, size);
    return mallocVal;
}
void *realloc(void *old, size_t size) {
    // recording the smaller size
    size_t smallersize = size;
    if (*((size_t *)old - 1) < size)
        smallersize = *((size_t *)old - 1);

    free(old); // freeing it before so there will be less airgaps
    void *newmem = malloc(size);      // mallocing the new aegment
    memcpy(newmem, old, smallersize); // copying the data there

    return newmem;
}