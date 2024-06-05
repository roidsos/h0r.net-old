#ifndef PMM_H__
#define PMM_H__

#include <core/mm/mem.h>
#include "libk/bitmap.h"
#include <limine.h>

#define DIV_ROUND_UP(x, y) (x + (y - 1)) / y
#define ALIGN_UP(x, y) DIV_ROUND_UP(x, y) * y
#define ALIGN_DOWN(x, y) (x / y) * y

#define BIT_TO_PAGE(bit) ((usize)bit * PAGE_SIZE)
#define PAGE_TO_BIT(page) ((usize)page / PAGE_SIZE)

_bool pmm_init();
_bool free_pages(void* addr,usize num);
_bool lock_pages(void* addr,usize num);

usize get_free_RAM();
usize get_used_RAM();
usize get_total_RAM();

void* request_pages(usize num);
#endif
