#ifndef __PAGING_H__
#define __PAGING_H__

#include <core/libk/stdint.h>
#include <core/libk/stdbool.h>

//basic flag generalization
#define FLAGS_R (1 << 0)
#define FLAGS_W (1 << 1)
#define FLAGS_X (1 << 2)
#define FLAGS_U (1 << 3)

u64  vmm_create_pagemap();
void vmm_bind_pagemap(u64 pml4);
u64  vmm_get_pagemap();

_bool vmm_map_page(u64 pml4,u64 vaddr, u64 paddr, u64 flags);
_bool vmm_unmap_page(u64 pml4,u64 vaddr);
_bool vmm_edit_flags(u64 pml4,u64 vaddr, u64 flags);

_bool vmm_map_range(u64 pml4,u64 vaddr, u64 paddr, u64 npages, u64 flags);
_bool vmm_unmap_range(u64 pml4,u64 vaddr, u64 npages);

u64 vmm_virt_to_phys(u64 pml4,u64 vaddr);
#endif // __PAGING_H__