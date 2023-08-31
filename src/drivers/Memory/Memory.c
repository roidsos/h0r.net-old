#include "Memory.h"
#include <stddef.h>

const char* memmap_type_names[] = {
    "LIMINE_MEMMAP_USABLE",                 
    "LIMINE_MEMMAP_RESERVED",               
    "LIMINE_MEMMAP_ACPI_RECLAIMABLE",       
    "LIMINE_MEMMAP_ACPI_NVS",               
    "LIMINE_MEMMAP_BAD_MEMORY",             
    "LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE", 
    "LIMINE_MEMMAP_KERNEL_AND_MODULES",     
    "LIMINE_MEMMAP_FRAMEBUFFER", 
};


uint64_t CalculateTotalMemorySize(struct limine_memmap_response* memmap) {
    static uint64_t totalSize = 0;
    if(totalSize > 0) return totalSize;
    
    for (size_t i = 0; i < memmap->entry_count; i++) {
	    totalSize += memmap->entries[i]->length;
    }
    return totalSize;
}