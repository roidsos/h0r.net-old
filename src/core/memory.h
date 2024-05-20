#ifndef MEMORY_H
#define MEMORY_H

#include <core/kernel.h>
#include <libk/stdtypes.h>

extern volatile struct limine_memmap_request memmap_request;

struct limine_memmap_entry* get_memmap_entry_of_type(uint32_t type);
extern const char* memmap_type_names[];

#define VIRT_TO_PHYS(addr) (void*)(((void*)addr) - data.hhdm_off)
#define PHYS_TO_VIRT(addr) (void*)(((uint64_t)addr) + data.hhdm_off)

#endif
