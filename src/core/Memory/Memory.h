#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <types/stdtypes.h>
#include <limine.h>

struct limine_memmap_entry* get_memmap_entry_of_type(uint32_t type);
extern const char* memmap_type_names[];

#endif // __MEMORY_H__