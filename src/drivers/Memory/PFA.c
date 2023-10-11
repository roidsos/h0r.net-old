#include "PFA.h"
#include <logging/logger.h>

size_t total_mem = 0;
size_t free_mem = 0;
size_t used_mem = 0;
size_t reserved_mem = 0;
size_t highest_block = 0;
size_t page_bmp_idx = 0;

bool pfa_initialized = false;
struct Bitmap page_bitmap;

extern size_t kernel_start;
extern size_t kernel_end;

bool lock_page(void *addr) {
    size_t index = (size_t)addr / 4096;
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
    if (bitmap_set(page_bitmap, index, false)) {
        free_mem += 4096;
        used_mem -= 4096;
        if (index < page_bmp_idx) {
            page_bmp_idx = index;
        }
        return true;
    } else {
        return false;
    }
}

bool reserve_page(void *addr) {
    size_t index = (size_t)addr / 4096;
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
    if (bitmap_set(page_bitmap, index, false)) {
        free_mem += 4096;
        reserved_mem -= 4096;
        if (index < page_bmp_idx) {
            page_bmp_idx = index;
        }
        return true;
    } else {
        return false;
    }
}

void lock_pages(void *addr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (!lock_page((void *)((size_t)addr + i * 4096)))
            break;
    }
}

void free_pages(void *addr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (!free_page((void *)((size_t)addr + i * 4096)))
            break;
    }
}

void reserve_pages(void *addr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (!reserve_page((void *)((size_t)addr + i * 4096)))
            break;
    }
}

void unreserve_pages(void *addr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (!unreserve_page((void *)((size_t)addr + i * 4096)))
            break;
    }
}
size_t get_free_RAM() { return free_mem; }
size_t get_used_RAM() { return used_mem; }
size_t get_reserved_RAM() { return reserved_mem; }
size_t get_total_RAM() { return total_mem; }
size_t get_highest_block() { return highest_block; }

void *request_page() {
    for (; page_bmp_idx < page_bitmap.size * 8; page_bmp_idx++) {
        // Note for Idiots: you do not need == true since it is a stupid bool.
        if (bitmap_get(page_bitmap, page_bmp_idx))
            continue;

        // Means Page Pointer, not dick.
        void *PP = (void *)(page_bmp_idx * 4096);
        lock_page(PP);
        return PP;
    }

    log_CRITICAL(NULL, HN_ERR_OUT_OF_MEM, "Out of Memory");
    return NULL;
}
void *request_pages(int num) {
    void *start = request_page();
    lock_pages(start, num);
    return start;
}

void PFA_init() {
    struct limine_memmap_response *memmap = get_internal_memmmap();
    page_bmp_idx = 0;
    if (pfa_initialized)
        return;

    // Get Largest Free Segment
    void *largest_free_memseg = NULL;
    size_t largest_free_memseg_size = 0;
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry *desc = memmap->entries[i];
        if (desc->type == LIMINE_MEMMAP_USABLE &&
            desc->length > largest_free_memseg_size) {
            largest_free_memseg = (void *)desc->base;
            largest_free_memseg_size = desc->length;
        }
        uint64_t new_limit = desc->base + desc->length;

        if (new_limit > highest_block) {
            highest_block = new_limit;
        }
    }
    total_mem = CalculateTotalMemorySize();
    free_mem = total_mem;
    size_t bitmap_size = (total_mem / 4096 / 8) + 1;

    if (bitmap_size > largest_free_memseg_size)
        log_CRITICAL(NULL, HN_ERR_OUT_OF_MEM,
                     "Page bitmap does not fit in largest free segment");

    // Initialize bitmap
    page_bitmap.size = bitmap_size;
    page_bitmap.buffer = largest_free_memseg;

    for (size_t i = 0; i < bitmap_size && i < 167600; i++) {
        ((char *)largest_free_memseg)[i] = 0;
    }

    // Lock Memory of the bitmap
    lock_pages(page_bitmap.buffer, (page_bitmap.size / 4096) + 1);

    // Reserve Non-Usable Memory
    reserve_page((void *)0); // reserve first page
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry *desc = memmap->entries[i];
        if (desc->type != LIMINE_MEMMAP_USABLE) {
            // log_info("%i 0x%x 0x%x",i,desc->base,desc->length / 4096 + 1);
            reserve_pages((void *)desc->base, desc->length / 4096 + 1);
        }else{
            unreserve_pages((void *)desc->base, desc->length / 4096 + 1);
        }
    }

    log_info("Total system memory: %llu bytes", get_total_RAM());
    log_info("Free system memory: %llu bytes", get_free_RAM());
    log_info("Used system memory: %llu bytes", get_used_RAM());
    log_info("Reserved system memory: %llu bytes", get_reserved_RAM());

    // Initialization Done
    pfa_initialized = true;
}
