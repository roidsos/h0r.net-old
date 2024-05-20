#include "heap.h"
#include <core/mm/pmm.h>
#include <libk/string.h>
#include <utils/error.h>
#include <utils/log.h>
#include <vendor/printf.h>

void heap_init() {}
void *malloc(usize size) {
    log_trace("malloc(%d)\n", size);
    char *PP = (char *)request_pages((size / 4096) + 1);
    *((usize *)PP) = size;
    PP += sizeof(usize);
    log_trace("malloc returned 0x%p\n", PP);
    return (void *)PP;
}

void free(void *tofree) {
    log_trace("free(0x%p)\n", tofree);
    if(tofree == NULL) return;
    char *PP = (char *)tofree;
    PP -= sizeof(usize);
    usize size = *((usize *)PP);
    free_pages(PP, (size / 4096) + 1);
}
void *calloc(usize count, usize size) {
    void *malloced = malloc(count * size);
    memset(malloced, 0, count * size);
    return malloced;
}
void *realloc(void *old, usize size) {
    // recording the smaller size
    usize smallersize = size;
    if (*((usize *)old - 1) < size)
        smallersize = *((usize *)old - 1);

    free(old); // freeing it before so there will be less airgaps
    void *newmem = malloc(size);      // mallocing the new aegment
    memcpy(newmem, old, smallersize); // copying the data there

    return newmem;
}
void *realloc_plus(void *old, usize size, usize oldsize) {
    usize smaller_size = size;
    if (oldsize < size)
        smaller_size = oldsize;

    free(old);
    void *newmem = malloc(size);
    memcpy(newmem, old, smaller_size);
    return newmem;
}
