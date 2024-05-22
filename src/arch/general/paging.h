#ifndef __PAGING_H__
#define __PAGING_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>

//basic flag generalization
#define FLAGS_R (1 << 0)
#define FLAGS_W (1 << 1)
#define FLAGS_X (1 << 2)
#define FLAGS_U (1 << 3)

u64  vmm_create_pagetable();
void vmm_bind_pagetable(u64 pml4);
u64  vmm_get_pagetable();
u64 vmm_map_kernel();

_bool vmm_map_page(u64 pml4,u64 vaddr, u64 paddr, u64 flags);
_bool vmm_unmap_page(u64 pml4,u64 vaddr);
_bool vmm_edit_flags(u64 pml4,u64 vaddr, u64 flags);

_bool vmm_map_range(u64 pml4,u64 vaddr, u64 paddr, u64 size, u64 flags);
_bool vmm_unmap_range(u64 pml4,u64 vaddr, u64 size);

u64 vmm_virt_to_phys(u64 pml4,u64 vaddr);
#endif // __PAGING_H__