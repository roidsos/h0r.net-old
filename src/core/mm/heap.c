#include "heap.h"
#include <libk/string.h>
#include <vendor/printf.h>
#include <utils/log.h>
#include <utils/error.h>
#include <core/mm/pmm.h>

void heap_init() {
}
void *malloc(size_t size) {
    log_trace("malloc(%d)\n", size);
    char *PP = (char *)request_pages((size / 4096) + 1);
    *((size_t *)PP) = size;
    PP += sizeof(size_t);
    log_trace("malloc returned 0x%p\n", PP);
    return (void *)PP;
}

void free(void *tofree) {
    log_trace("free(0x%p)\n", tofree);
    char *PP = (char *)tofree;
    PP -= sizeof(size_t);
    size_t size = *((size_t *)PP);
    free_pages(PP, (size / 4096) + 1);
}
void *calloc(size_t count, size_t size) {
    void *malloced = malloc(count * size);
    memset(malloced, 0, count * size);
    return malloced;
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
void *realloc_plus(void *old, size_t size, size_t oldsize) {
    size_t smaller_size = size;
    if (oldsize < size)
        smaller_size = oldsize;

    free(old);
    void *newmem = malloc(size);
    memcpy(newmem, old, smaller_size);
    return newmem;
}
