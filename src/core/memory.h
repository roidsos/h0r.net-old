#ifndef MEMORY_H
#define MEMORY_H

#include <core/kernel.h>
#include <types/stdtypes.h>

#define VIRT_TO_PHYS(addr) (void*)(((uint64_t)addr) - data.hhdm_off)
#define PHYS_TO_VIRT(addr) (void*)(((uint64_t)addr) + data.hhdm_off)

#endif
