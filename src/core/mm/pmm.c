#include "pmm.h"
#include <config.h>
#include <string.h>
#include <utils/error.h>
#include <utils/log.h>
#include <vendor/printf.h>

usize total_mem = 0;
usize free_mem = 0;
usize used_mem = 0;
usize highest_block = 0;

struct Bitmap page_bmp;

#define BIT_TO_PAGE(bit) ((usize)bit * PAGE_SIZE)
#define PAGE_TO_BIT(page) ((usize)page / PAGE_SIZE)

extern usize kernel_start;
extern usize kernel_end;

_bool lock_page(void *addr) {
    usize index = (usize)addr / PAGE_SIZE;
    if (bitmap_get(page_bmp, index)) {
        return true;
    }
    if (bitmap_set(page_bmp, index, true)) {
        free_mem -= PAGE_SIZE;
        used_mem += PAGE_SIZE;
        return true;
    } else {
        return false;
    }
}
_bool free_page(void *addr) {
    usize index = (usize)addr / PAGE_SIZE;
    if (!bitmap_get(page_bmp, index)) {
        return true;
    }
    if (bitmap_set(page_bmp, index, false)) {
        free_mem += PAGE_SIZE;
        used_mem -= PAGE_SIZE;
        return true;
    } else {
        return false;
    }
}
_bool reserve_page(void *addr) {
    usize index = (usize)addr / PAGE_SIZE;
    if (bitmap_get(page_bmp, index)) {
        return true;
    }
    if (bitmap_set(page_bmp, index, true)) {
        return true;
    } else {
        return false;
    }
}
_bool unreserve_page(void *addr) {
    usize index = (usize)addr / PAGE_SIZE;
    if (!bitmap_get(page_bmp, index)) {
        return true;
    }
    if (bitmap_set(page_bmp, index, false)) {
    } else {
        return false;
    }
    return true;
}
_bool lock_pages(void *addr, usize num) {
    for (usize i = 0; i < num; i++) {
        if (!lock_page((void *)(addr + i * PAGE_SIZE)))
            return false;
    }
    return true;
}
_bool free_pages(void *addr, usize num) {
    for (usize i = 0; i < num; i++) {
        if (!free_page((void *)(addr + i * PAGE_SIZE)))
            return false;
    }
    return true;
}
_bool reserve_pages(void *addr, usize num) {
    for (usize i = 0; i < num; i++) {
        if (!reserve_page((void *)(addr + i * PAGE_SIZE)))
            return false;
    }
    return true;
}
_bool unreserve_pages(void *addr, usize num) {
    for (usize i = 0; i < num; i++) {
        if (!unreserve_page((void *)(addr + i * PAGE_SIZE)))
            return false;
    }
    return true;
}

usize get_free_RAM() { return free_mem; }
usize get_used_RAM() { return used_mem; }
usize get_total_RAM() { return total_mem; }

static void *find_free_range(usize npages) {
    for (usize addr = 0; addr <= page_bmp.size; addr++) {
        for (usize page = 0; page <= npages; page++) {
            if (bitmap_get(page_bmp, addr + PAGE_TO_BIT(page)))
                break;

            if (page == npages)
                return (void *)BIT_TO_PAGE(addr);
        }
    }

    trigger_psod(HN_ERR_OUT_OF_MEM, "Out of Memory", NULL);
    return NULL;
}

void *request_pages(usize num) {
    log_trace("Allocating %d pages\n", num);
    void *PP = (void *)find_free_range(num);
    if (!lock_pages(PP, num)) {
        return NULL;
    }
    return PP;
}

void pmm_init() {
    usize page_bmp_size = 0;
    void *largest_free_memseg = NULL;
    usize largest_free_memseg_size = 0;
    log_trace("Memory map segments:\n");
    for (usize i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *desc = memmap_request.response->entries[i];
        log_trace("  -type:\"%s\",base:0x%p,length:%u\n",
                  memmap_type_names[desc->type], desc->base, desc->length);
        if (desc->type == LIMINE_MEMMAP_USABLE &&
            desc->length > largest_free_memseg_size) {
            largest_free_memseg = (void *)(desc->base);
            largest_free_memseg_size = desc->length;
        }

        if (desc->type == LIMINE_MEMMAP_USABLE || desc->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE || desc->type == LIMINE_MEMMAP_ACPI_RECLAIMABLE) {
            free_mem += desc->length;
        }
        page_bmp_size += (desc->length / (PAGE_SIZE * 8)) + 1;
    }
    total_mem = free_mem;
    if (page_bmp_size > largest_free_memseg_size)
        trigger_psod(HN_ERR_OUT_OF_MEM,
                     "Page bitmap does not fit in largest free segment", NULL);

    page_bmp.size = page_bmp_size;
    page_bmp.buffer = largest_free_memseg;
    
    memset(page_bmp.buffer, 0xff, page_bmp.size);
    for (usize i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *desc = memmap_request.response->entries[i];
        if (desc->type == LIMINE_MEMMAP_USABLE || desc->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE || desc->type == LIMINE_MEMMAP_ACPI_RECLAIMABLE) {
            unreserve_pages((void *)desc->base, desc->length / PAGE_SIZE + 1);
        } else {
            reserve_pages((void *)desc->base, desc->length / PAGE_SIZE + 1);
        }
    }
    lock_pages(page_bmp.buffer, (page_bmp.size / PAGE_SIZE) + 1);

    log_trace("PMM initialized\n");
    log_trace("Total RAM: %u KB\n", total_mem / 1024);
    log_trace("Free RAM: %u KB\n", free_mem / 1024);
    log_trace("Used RAM: %u KB\n", used_mem / 1024);
}
