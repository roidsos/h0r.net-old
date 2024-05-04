#ifndef PMM_H__
#define PMM_H__

#include <core/memory.h>
#include "types/bitmap.h"
#include <limine.h>

void pmm_init();
bool free_pages(void* addr,size_t num);
bool lock_pages(void* addr,size_t num);

size_t get_free_RAM();
size_t get_used_RAM();
size_t get_total_RAM();

void* request_pages(size_t num);
#endif
