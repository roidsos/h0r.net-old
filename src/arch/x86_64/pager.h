#ifndef PAGER_H
#define PAGER_H

#include <libk/stdint.h>
#include <libk/stdbool.h>

#define VMM_BITMASK_L4_PRESENT 1 << 0
#define VMM_BITMASK_L4_WRITABLE 1 << 1
#define VMM_BITMASK_L4_USER 1 << 2
#define VMM_BITMASK_L4_WRITE_THROUGH 1 << 3
#define VMM_BITMASK_L4_CACHE_DISABLE 1 << 4
#define VMM_BITMASK_L4_ACCESSED 1 << 5
#define VMM_BITMASK_L4_AVAILABLE 1 << 6
#define VMM_BITMASK_L4_PAGE_SIZE 1 << 7
#define VMM_BITMASK_L4_AVAILABLE2 1 << 8 || 1 << 9 || 1 << 10 || 1 << 11
#define VMM_BITMASK_L4_ADDR 0x000FFFFFFFFFF000
#define VMM_BITMASK_L4_AVAILABLE3 0x7FF0000000000000
#define VMM_BITMASK_L4_XD 1 << 63

#define PML4_GET_ADDR(VALUE) ((VALUE) & VMM_BITMASK_L4_ADDR)
#define PML4_GET_FLAGS(VALUE) ((VALUE) & ~VMM_BITMASK_L4_ADDR)


u64 vmm_create_pagetable();
void     vmm_bind_pagetable(u64 pml4);
u64 vmm_get_pagetable();

_bool vmm_map_page(u64 vaddr, u64 paddr, u64 flags);
_bool vmm_unmap_page(u64 vaddr);
_bool vmm_edit_flags(u64 vaddr, u64 flags);

u64 vmm_virt_to_phys(u64 vaddr,u64 pml4);

#endif