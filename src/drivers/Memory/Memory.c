#include "Memory.h"
#include <stddef.h>

struct limine_memmap_response* internal_memmap;

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
void mem_init(struct limine_memmap_response* memmap){
    internal_memmap = memmap;
}
struct limine_memmap_entry* get_memmap_entry_of_type(int type){
    for (size_t i = 0; i < internal_memmap->entry_count; i++)
    {
        if(internal_memmap->entries[i]->type == type){
            return internal_memmap->entries[i];
        }
    }
    
}

uint64_t CalculateTotalMemorySize() {
    static uint64_t totalSize = 0;
    if(totalSize > 0) return totalSize;
    
    for (size_t i = 0; i < internal_memmap->entry_count; i++) {
	    totalSize += internal_memmap->entries[i]->length;
    }
    return totalSize;
}