#include "pager.h"
#include <core/memory.h>
#include <core/mm/pmm.h>
#include <klibc/string.h>

uint64_t vmm_create_pagetable() { return 0; }

void vmm_bind_pagetable(uint64_t pml4) {
    __asm__ volatile("mov %0, %%cr3" : : "r"(pml4));
}
uint64_t vmm_get_pagetable() {
    uint64_t pml4;
    __asm__ volatile("mov %%cr3, %0" : "=r"(pml4));
    return pml4;
}

uintptr_t *get_next_lvl(uintptr_t *lvl, size_t index, uint64_t flags,
                        bool alloc) {

    if (lvl[index] & VMM_BITMASK_L4_PRESENT) {
        return PHYS_TO_VIRT(PML4_GET_ADDR(lvl[index]));
    }
    if (alloc) {
        uintptr_t *new_lvl = (uintptr_t *)request_pages(1);
        memset(new_lvl, 0, 0x1000);
        lvl[index] = (uint64_t)VIRT_TO_PHYS(new_lvl) | flags;
        return PHYS_TO_VIRT(new_lvl);
    }
    return 0;
}

bool vmm_map_page(uint64_t vaddr, uint64_t paddr, uint64_t flags) {
    size_t pml4_index = (vaddr & ((uintptr_t)0x1ff << 39)) >> 39;
    size_t pml3_index = (vaddr & ((uintptr_t)0x1ff << 30)) >> 30;
    size_t pml2_index = (vaddr & ((uintptr_t)0x1ff << 21)) >> 21;
    size_t pml1_index = (vaddr & ((uintptr_t)0x1ff << 12)) >> 12;

    uintptr_t *pml3 =
        get_next_lvl((uintptr_t *)vmm_get_pagetable(), pml4_index, flags, true);
    uintptr_t *pml2 = get_next_lvl(pml3, pml3_index, flags, true);
    uintptr_t *pml1 = get_next_lvl(pml2, pml2_index, flags, true);
    pml1[pml1_index] = paddr | flags;
    return true;
}
bool vmm_unmap_page(uint64_t vaddr) {
    size_t pml4_index = (vaddr & ((uintptr_t)0x1ff << 39)) >> 39;
    size_t pml3_index = (vaddr & ((uintptr_t)0x1ff << 30)) >> 30;
    size_t pml2_index = (vaddr & ((uintptr_t)0x1ff << 21)) >> 21;
    size_t pml1_index = (vaddr & ((uintptr_t)0x1ff << 12)) >> 12;

    uintptr_t *pml4 = (uintptr_t *)vmm_get_pagetable();
    if (!(pml4[pml4_index] & VMM_BITMASK_L4_PRESENT)) {
        return false;
    }
    uintptr_t *pml3 = (uintptr_t *)(pml4[pml4_index] & ~0xFFF);
    if (!(pml3[pml3_index] & VMM_BITMASK_L4_PRESENT)) {
        return false;
    }
    uintptr_t *pml2 = (uintptr_t *)(pml3[pml3_index] & ~0xFFF);
    if (!(pml2[pml2_index] & VMM_BITMASK_L4_PRESENT)) {
        return false;
    }
    uintptr_t *pml1 = (uintptr_t *)(pml2[pml2_index] & ~0xFFF);
    pml1[pml1_index] = 0;
    return true;
}

uint64_t vmm_virt_to_phys(uint64_t vaddr, uint64_t pml4) {
    size_t pml4_index = (vaddr & ((uintptr_t)0x1ff << 39)) >> 39;
    size_t pml3_index = (vaddr & ((uintptr_t)0x1ff << 30)) >> 30;
    size_t pml2_index = (vaddr & ((uintptr_t)0x1ff << 21)) >> 21;
    size_t pml1_index = (vaddr & ((uintptr_t)0x1ff << 12)) >> 12;

    uintptr_t *pml3 = get_next_lvl((uint64_t *)pml4, pml4_index, 0, true);
    uintptr_t *pml2 = get_next_lvl(pml3, pml3_index, 0, true);
    uintptr_t *pml1 = get_next_lvl(pml2, pml2_index, 0, true);
    return PML4_GET_ADDR(pml1[pml1_index]);
}
