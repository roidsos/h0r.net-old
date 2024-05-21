#ifndef MEMORY_H
#define MEMORY_H

#include <core/kernel.h>
#include <libk/stdint.h>

//WARN: Arch specific code in core/ + TODO: smh move to arch/
extern volatile struct limine_memmap_request memmap_request;

struct limine_memmap_entry* get_memmap_entry_of_type(u32 type);
extern const char* memmap_type_names[];

#define VIRT_TO_PHYS(addr) (void*)(((void*)addr) - data.hhdm_off)
#define PHYS_TO_VIRT(addr) (void*)(((u64)addr) + data.hhdm_off)

#endif
