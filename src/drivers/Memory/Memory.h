#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <limine.h>

uint64_t CalculateTotalMemorySize(struct limine_memmap_response* memmap);
extern const char* memmap_type_names[];

#endif // __MEMORY_H__