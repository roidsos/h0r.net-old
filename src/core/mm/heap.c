#include "heap.h"
#include "pmm.h"
#include <core/config.h>
#include <klibc/string.h>
#include <vendor/printf.h>

#define MAX_PAGES                                                              \
    (4096 * SLAB_PAGES_TO_STORE_FUCKING_PAGES_WITH) / sizeof(page_t)

typedef struct {
    uint64_t paddr;
    uint16_t fullness;
} page_t;

typedef struct {
    page_t *page;
    size_t page_id;
    size_t size;
} heap_metadata_t;

page_t *pages;
size_t num_used_pages;

void heap_init() {
    pages = (page_t *)PHYS_TO_VIRT(
        request_pages(SLAB_PAGES_TO_STORE_FUCKING_PAGES_WITH));
    num_used_pages = 0;
    memset(pages, 0, SLAB_PAGES_TO_STORE_FUCKING_PAGES_WITH * 4096);
}

void *malloc(size_t size) {
    page_t *curpage = &pages[num_used_pages];
    size_t size_remainder = size;

    if (!curpage->paddr) // needing this redundant check, since we use
                         // curpage->paddr to calcualte PP
        curpage->paddr = (uint64_t)request_pages(1);

    char *PP = PHYS_TO_VIRT(curpage->paddr) + curpage->fullness;

recheck:
    if (!curpage->paddr)
        curpage->paddr = (uint64_t)request_pages(1);

    if (curpage->fullness + size_remainder > 4096) {
        curpage->fullness += size_remainder;
        size_remainder -= 4096 - (curpage->fullness + size_remainder);
        curpage = &pages[num_used_pages++];
        goto recheck;
    }
    curpage->fullness += size_remainder;

    *((heap_metadata_t *)PP) = (heap_metadata_t){curpage, num_used_pages, size};
    PP += sizeof(heap_metadata_t);
    return (void *)PP;
}

void free(void *tofree) {
    char *PP = (char *)tofree;
    PP -= sizeof(heap_metadata_t);
    heap_metadata_t hdata = *((heap_metadata_t *)PP);
    size_t old_fullness = 0;
    size_t size_remainder = hdata.size;

recheck:
    old_fullness = hdata.page->fullness;
    hdata.page->fullness -= size_remainder;
    if (hdata.page->fullness >
        old_fullness) { // checking whether it went bellow 0 via overflow
                        // check(since size_t is unsigned)
        free_pages((void *)hdata.page->paddr, 1);
        size_remainder -= SIZE_MAX - hdata.page->fullness;
        hdata.page->paddr = 0;
        hdata.page_id--;
        hdata.page = &pages[hdata.page_id];
        goto recheck;
    }
}
void *calloc(size_t size) {
    void *mallocVal = malloc(size);
    memset(mallocVal, 0, size);
    return mallocVal;
}
void *realloc(void *old, size_t size) {
    if (old == NULL) {
        return malloc(size);
    }
    free(old);
    void *newmem = malloc(size);

    size_t smallersize = size;
    if (((heap_metadata_t *)old - 1)->size < size)
        smallersize = ((heap_metadata_t *)old - 1)->size;

    memcpy(newmem, old, smallersize);

    return newmem;
}
