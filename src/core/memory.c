#include "memory.h"

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};

const char *memmap_type_names[] = {
    "LIMINE_MEMMAP_USABLE",
    "LIMINE_MEMMAP_RESERVED",
    "LIMINE_MEMMAP_ACPI_RECLAIMABLE",
    "LIMINE_MEMMAP_ACPI_NVS",
    "LIMINE_MEMMAP_BAD_MEMORY",
    "LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE",
    "LIMINE_MEMMAP_KERNEL_AND_MODULES",
    "LIMINE_MEMMAP_FRAMEBUFFER",
};
struct limine_memmap_entry *get_memmap_entry_of_type(u32 type) {
    for (usize i = 0; i < memmap_request.response->entry_count; i++) {
        if (memmap_request.response->entries[i]->type == type) {
            return memmap_request.response->entries[i];
        }
    }
    return NULL;
}
