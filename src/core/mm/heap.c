#include <vendor/printf.h>
#include "heap.h"
#include "pmm.h"
#include <klibc/string.h>

void *malloc(size_t size) {
    char *PP = (char *)request_pages((size / 4096) + 1);
    *((size_t *)PP) = size;
    PP += sizeof(size_t);
    return (void *)PP;
}

void free(void *tofree) {
    char *PP = (char *)tofree;
    PP -= sizeof(size_t);
    size_t size = *((size_t *)PP);
    free_pages(PP, (size / 4096) + 1);
}
void *calloc(size_t size) {
    void *mallocVal = malloc(size);
    memset(mallocVal, 0, size);
    return mallocVal;
}
void *realloc(void *old, size_t size) {
    if (old == NULL){
        return malloc(size);
    }
    free(old);
    void *newmem = malloc(size);

    size_t smallersize = size;
    if (*((size_t *)old - 1) < size)
        smallersize = *((size_t *)old - 1);

    memcpy(newmem, old, smallersize);

    return newmem;
}
