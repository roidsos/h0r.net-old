#include "pmm.h"
#include <utils/error.h>
#include <vendor/printf.h>

size_t total_mem = 0;
size_t free_mem = 0;
size_t used_mem = 0;
size_t reserved_mem = 0;
size_t highest_block = 0;

struct Bitmap page_bitmap;

#define BIT_TO_PAGE(bit) ((size_t)bit * 4096)
#define PAGE_TO_BIT(page) ((size_t)page / 4096)

extern size_t kernel_start;
extern size_t kernel_end;

bool lock_page(void *addr) {
    size_t index = (size_t)addr / 4096;
    if (bitmap_get(page_bitmap, index)) {
        return true;
    }
    if (bitmap_set(page_bitmap, index, true)) {
        free_mem -= 4096;
        used_mem += 4096;
        return true;
    } else {
        return false;
    }
}
bool free_page(void *addr) {
    size_t index = (size_t)addr / 4096;
    if (!bitmap_get(page_bitmap, index)) {
        return true;
    }
    if (bitmap_set(page_bitmap, index, false)) {
        free_mem += 4096;
        used_mem -= 4096;
        return true;
    } else {
        return false;
    }
}
bool reserve_page(void *addr) {
    size_t index = (size_t)addr / 4096;
    if (bitmap_get(page_bitmap, index)) {
        return true;
    }
    if (bitmap_set(page_bitmap, index, true)) {
        free_mem -= 4096;
        reserved_mem += 4096;
        return true;
    } else {
        return false;
    }
}
bool unreserve_page(void *addr) {
    size_t index = (size_t)addr / 4096;
    if (!bitmap_get(page_bitmap, index)) {
        return true;
    }
    if (bitmap_set(page_bitmap, index, false)) {
        free_mem += 4096;
        reserved_mem -= 4096;
    } else {
        return false;
    }
    return true;
}
bool lock_pages(void *addr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (!lock_page((void *)(addr + i * 4096)))
            return false;
    }
    return true;
}
bool free_pages(void *addr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (!free_page((void *)(addr + i * 4096)))
            return false;
    }
    return true;
}
bool reserve_pages(void *addr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (!reserve_page((void *)(addr + i * 4096)))
            return false;
    }
    return true;
}
bool unreserve_pages(void *addr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (!unreserve_page((void *)(addr + i * 4096)))
            return false;
    }
    return true;
}

size_t get_free_RAM() { return free_mem; }
size_t get_used_RAM() { return used_mem; }
size_t get_total_RAM() { return total_mem; }

static void *find_free_range(size_t npages) {
    for (size_t addr = 0; addr <= page_bitmap.size; addr++) {
        for (size_t page = 0; page < npages; page++) {
            if (bitmap_get(page_bitmap, addr + PAGE_TO_BIT(page)))
                break;

            if (page == npages - 1)
                return (void *)BIT_TO_PAGE(addr);
        }
    }

    trigger_psod(HN_ERR_OUT_OF_MEM, "Out of Memory", NULL);
    return NULL;
}

void *request_pages(size_t num) {
    void *PP = (void *)find_free_range(num);
    if (!lock_pages(PP, num)) {
        return NULL;
    }
    return PP;
}

void pmm_init() {
    size_t page_bmp_size = 0;
    void *largest_free_memseg = NULL;
    size_t largest_free_memseg_size = 0;
    for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *desc = memmap_request.response->entries[i];
        if (desc->type == LIMINE_MEMMAP_USABLE &&
            desc->length > largest_free_memseg_size) {
            largest_free_memseg = (void *)(desc->base);
            largest_free_memseg_size = desc->length;
            total_mem += desc->length;
        }
        page_bmp_size += (desc->length / (4096 * 8)) + 1;
    }
    free_mem = total_mem;

    if (page_bmp_size > largest_free_memseg_size)
        trigger_psod(HN_ERR_OUT_OF_MEM,
                     "Page bitmap does not fit in largest free segment", NULL);

    page_bitmap.size = page_bmp_size;
    page_bitmap.buffer = largest_free_memseg;

    lock_pages(page_bitmap.buffer, (page_bitmap.size / 4096) + 1);
    reserve_page((void *)0);
    for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *desc = memmap_request.response->entries[i];
        if (desc->type != LIMINE_MEMMAP_USABLE) {
            reserve_pages((void *)desc->base, desc->length / 4096 + 1);
        } else {
            unreserve_pages((void *)desc->base, desc->length / 4096 + 1);
        }
    }
}
