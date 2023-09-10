#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <limine.h>

struct limine_memmap_response *get_internal_memmmap();
uint64_t CalculateTotalMemorySize();
void mem_init();
struct limine_memmap_entry* get_memmap_entry_of_type(uint32_t type);
extern const char* memmap_type_names[];

#endif // __MEMORY_H__