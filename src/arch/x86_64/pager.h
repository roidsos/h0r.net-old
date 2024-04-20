#ifndef PAGER_H
#define PAGER_H

#include <types/stdtypes.h>

#define BITMASK_L4_PRESENT 1 << 0
#define BITMASK_L4_WRITABLE 1 << 1
#define BITMASK_L4_USER 1 << 2
#define BITMASK_L4_WRITE_THROUGH 1 << 3
#define BITMASK_L4_CACHE_DISABLE 1 << 4
#define BITMASK_L4_ACCESSED 1 << 5
#define BITMASK_L4_AVAILABLE 1 << 6
#define BITMASK_L4_PAGE_SIZE 1 << 7
#define BITMASK_L4_AVAILABLE2 1 << 8 || 1 << 9 || 1 << 10 || 1 << 11
#define BITMASK_L4_ADDR 0x000FFFFFFFFFF000
#define BITMASK_L4_AVAILABLE3 0x7FF0000000000000
#define BITMASK_L4_XD 1 << 63

uint64_t create_pagetable();
void bind_pagetable(uint64_t pml4);
uint64_t get_pagetable();

bool map_page(uint64_t vaddr, uint64_t paddr, uint64_t flags);
bool unmap_page(uint64_t vaddr);

uint64_t virt_to_phys(uint64_t vaddr,uint64_t pml4);
uint64_t phys_to_virt(uint64_t paddr,uint64_t pml4);

#endif