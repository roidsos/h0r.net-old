#ifndef PAGER_H
#define PAGER_H

#include <types/stdtypes.h>

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


uint64_t vmm_create_pagetable();
void     vmm_bind_pagetable(uint64_t pml4);
uint64_t vmm_get_pagetable();

bool vmm_map_page(uint64_t vaddr, uint64_t paddr, uint64_t flags);
bool vmm_unmap_page(uint64_t vaddr);
bool vmm_edit_flags(uint64_t vaddr, uint64_t flags);

uint64_t vmm_virt_to_phys(uint64_t vaddr,uint64_t pml4);

#endif