#include "pmm.h"
#include <config.h>
#include <core/error.h>
#include <string.h>
#include <utils/log.h>
#include <vendor/printf.h>

usize total_mem = 0;
usize free_mem = 0;
usize used_mem = 0;
usize highest_block = 0;

struct Bitmap page_bmp;

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
    void *PP = (void *)find_free_range(num);
    if (!lock_pages(PP, num)) {
        return NULL;
    }
    return PP;
}
// WARN: Arch specific code in core/ + TODO: smh move to arch/ or make
// architecture neutral
volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};

_bool pmm_init() {
    usize page_bmp_size = 0;
    void *largest_free_memseg = NULL;
    usize largest_free_memseg_size = 0;
    for (usize i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap_request.response->entries[i];
        page_bmp_size += (entry->length / (PAGE_SIZE * 8)) + 1;
        if (entry->type == LIMINE_MEMMAP_USABLE ||
            entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE ||
            entry->type == LIMINE_MEMMAP_ACPI_RECLAIMABLE) {
            free_mem += entry->length;
            if (entry->length > largest_free_memseg_size) {
                largest_free_memseg = (void *)(entry->base);
                largest_free_memseg_size = entry->length;
            }
        }
    }

    total_mem = free_mem;
    if (page_bmp_size > largest_free_memseg_size)
        trigger_psod(HN_ERR_OUT_OF_MEM,
                     "Page bitmap does not fit in largest free segment", NULL);
    page_bmp.size = ALIGN_UP(page_bmp_size, PAGE_SIZE);
    page_bmp.buffer = largest_free_memseg;

    memset(page_bmp.buffer, 0xff, page_bmp.size);
    for (usize i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap_request.response->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE ||
            entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE ||
            entry->type == LIMINE_MEMMAP_ACPI_RECLAIMABLE) {
            usize index = entry->base / PAGE_SIZE;
            for (usize j = 0; j < DIV_ROUND_UP(entry->length, PAGE_SIZE); j++) {
                bitmap_set(page_bmp, index + j, false);
            }
        }
    }
    lock_pages(page_bmp.buffer, DIV_ROUND_UP(page_bmp.size, PAGE_SIZE));

    log_trace("PMM initialized\n");
    log_trace("Total RAM: %u KB\n", total_mem / 1024);
    log_trace("Free RAM: %u KB\n", free_mem / 1024);
    log_trace("Used RAM: %u KB\n", used_mem / 1024);
    return true;
}
