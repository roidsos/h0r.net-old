#ifndef PMM_H__
#define PMM_H__

#include <core/memory.h>
#include "libk/bitmap.h"
#include <limine.h>

void pmm_init();
_bool free_pages(void* addr,usize num);
_bool lock_pages(void* addr,usize num);

usize get_free_RAM();
usize get_used_RAM();
usize get_total_RAM();

void* request_pages(usize num);
#endif
