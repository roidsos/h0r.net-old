#include "pager.h"
#include <core/memory.h>
#include <core/mm/pmm.h>
#include <libk/string.h>
#include <stdint.h>

u64 vmm_create_pagetable() { return 0; }

void vmm_bind_pagetable(u64 pml4) {
    __asm__ volatile("mov %0, %%cr3" : : "r"(pml4));
}
u64 vmm_get_pagetable() {
    u64 pml4;
    __asm__ volatile("mov %%cr3, %0" : "=r"(pml4));
    return pml4;
}

uptr *get_next_lvl(uptr *lvl, usize index, u64 flags, _bool alloc) {

    if (lvl[index] & VMM_BITMASK_L4_PRESENT) {
        return PHYS_TO_VIRT(PML4_GET_ADDR(lvl[index]));
    }
    if (alloc) {
        uptr *new_lvl = (uptr *)request_pages(1);
        memset(new_lvl, 0, 0x1000);
        lvl[index] = (u64)VIRT_TO_PHYS(new_lvl) | flags;
        return PHYS_TO_VIRT(new_lvl);
    }
    return 0;
}

_bool vmm_map_page(u64 vaddr, u64 paddr, u64 flags) {
    usize pml4_index = (vaddr & ((uptr)0x1ff << 39)) >> 39;
    usize pml3_index = (vaddr & ((uptr)0x1ff << 30)) >> 30;
    usize pml2_index = (vaddr & ((uptr)0x1ff << 21)) >> 21;
    usize pml1_index = (vaddr & ((uptr)0x1ff << 12)) >> 12;

    uptr *pml3 =
        get_next_lvl((uptr *)vmm_get_pagetable(), pml4_index, flags, true);
    uptr *pml2 = get_next_lvl(pml3, pml3_index, flags, true);
    uptr *pml1 = get_next_lvl(pml2, pml2_index, flags, true);
    pml1[pml1_index] = paddr | flags;
    return true;
}
_bool vmm_unmap_page(u64 vaddr) {
    usize pml4_index = (vaddr & ((uptr)0x1ff << 39)) >> 39;
    usize pml3_index = (vaddr & ((uptr)0x1ff << 30)) >> 30;
    usize pml2_index = (vaddr & ((uptr)0x1ff << 21)) >> 21;
    usize pml1_index = (vaddr & ((uptr)0x1ff << 12)) >> 12;

    uptr *pml4 = (uptr *)vmm_get_pagetable();
    if (!(pml4[pml4_index] & VMM_BITMASK_L4_PRESENT)) {
        return false;
    }
    uptr *pml3 = (uptr *)(pml4[pml4_index] & ~0xFFF);
    if (!(pml3[pml3_index] & VMM_BITMASK_L4_PRESENT)) {
        return false;
    }
    uptr *pml2 = (uptr *)(pml3[pml3_index] & ~0xFFF);
    if (!(pml2[pml2_index] & VMM_BITMASK_L4_PRESENT)) {
        return false;
    }
    uptr *pml1 = (uptr *)(pml2[pml2_index] & ~0xFFF);
    pml1[pml1_index] = 0;
    return true;
}

u64 vmm_virt_to_phys(u64 vaddr, u64 pml4) {
    usize pml4_index = (vaddr & ((uptr)0x1ff << 39)) >> 39;
    usize pml3_index = (vaddr & ((uptr)0x1ff << 30)) >> 30;
    usize pml2_index = (vaddr & ((uptr)0x1ff << 21)) >> 21;
    usize pml1_index = (vaddr & ((uptr)0x1ff << 12)) >> 12;

    uptr *pml3 = get_next_lvl((uptr *)pml4, pml4_index, 0, true);
    uptr *pml2 = get_next_lvl(pml3, pml3_index, 0, true);
    uptr *pml1 = get_next_lvl(pml2, pml2_index, 0, true);
    return PML4_GET_ADDR(pml1[pml1_index]);
}
