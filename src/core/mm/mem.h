#ifndef MEMORY_H
#define MEMORY_H

#include <core/kernel.h>
#include <core/libk/stdint.h>

#define VIRT_TO_PHYS(addr) (void*)(((void*)addr) - hn_data.hhdm_off)
#define PHYS_TO_VIRT(addr) (void*)(((u64)addr) + hn_data.hhdm_off)

#endif
