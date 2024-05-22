#include "pager.h"
#include "config.h"
#include <libk/stdint.h>
#include <libk/stdbool.h>
#include <core/mm/mem.h>
#include <core/mm/pmm.h>
#include <arch/general/paging.h>
#include <libk/string.h>
#include <core/kernel.h>
#include <utils/log.h>
#include <stdint.h>

u64 transform_flags(u64 flags) {
    u64 new_flags = (u64)X86_PAGER_BITMASK_XD;
    if (flags & FLAGS_R) {
        new_flags |= X86_PAGER_BITMASK_PRESENT;
    }
    if (flags & FLAGS_W) {
        new_flags |= X86_PAGER_BITMASK_WRITABLE;
    }
    if (flags & FLAGS_X) {
        new_flags &= ~(u64)X86_PAGER_BITMASK_XD;
    }
    if (flags & FLAGS_U) {
        new_flags |= X86_PAGER_BITMASK_USER;
    }
    return new_flags;
}

uptr *get_next_lvl(uptr *lvl, usize index, u64 flags, _bool alloc) {

    if (lvl[index] & X86_PAGER_BITMASK_PRESENT) {
        return PHYS_TO_VIRT(PML4_GET_ADDR(lvl[index]));
    }
    if (alloc) {
        uptr *new_lvl = (uptr *)PHYS_TO_VIRT(request_pages(1));
        memset(new_lvl, 0, PAGE_SIZE);
        lvl[index] = (u64)VIRT_TO_PHYS(new_lvl) | flags;
        return new_lvl; 
    }
    return 0;
}

u64 vmm_create_pagetable() { 
    uptr *pml4 = (uptr *)PHYS_TO_VIRT(request_pages(1));
    memset(pml4, 0, PAGE_SIZE);
    return (u64)pml4;
}

void vmm_bind_pagetable(u64 pml4) {
    __asm__ volatile("mov %0, %%cr3" : : "r"(pml4));
}
u64 vmm_get_pagetable() {
    u64 pml4;
    __asm__ volatile("mov %%cr3, %0" : "=r"(pml4));
    return pml4;
}

_bool vmm_map_page(u64 pml4,u64 vaddr, u64 paddr, u64 flags) {
    usize pml4_index = (vaddr >> 39) & 0x1ff;
    usize pml3_index = (vaddr >> 30) & 0x1ff;
    usize pml2_index = (vaddr >> 21) & 0x1ff;
    usize pml1_index = (vaddr >> 12) & 0x1ff;

    uptr *pml3 =
        get_next_lvl((uptr *)pml4, pml4_index, transform_flags(flags), true);
    uptr *pml2 = get_next_lvl(pml3, pml3_index, transform_flags(flags), true);
    uptr *pml1 = get_next_lvl(pml2, pml2_index, transform_flags(flags), true);
    pml1[pml1_index] = paddr | transform_flags(flags);
    return true;
}

_bool vmm_edit_flags(u64 pml4,u64 vaddr, u64 flags) {
    usize pml4_index = (vaddr >> 39) & 0x1ff;
    usize pml3_index = (vaddr >> 30) & 0x1ff;
    usize pml2_index = (vaddr >> 21) & 0x1ff;
    usize pml1_index = (vaddr >> 12) & 0x1ff;

    uptr *pml3 =
        get_next_lvl((uptr *)pml4, pml4_index, transform_flags(flags), true);
    uptr *pml2 = get_next_lvl(pml3, pml3_index, transform_flags(flags), true);
    uptr *pml1 = get_next_lvl(pml2, pml2_index, transform_flags(flags), true);
    pml1[pml1_index] = PML4_GET_ADDR(pml1[pml1_index]) | transform_flags(flags);
    return true;
}


_bool vmm_map_range(u64 pml4,u64 vaddr, u64 paddr, u64 size, u64 flags){
    _bool success = true;
    for (u64 i = 0; i < size; i += PAGE_SIZE) {
        log_trace("Mapping 0x%p -> 0x%p\n", vaddr + i, paddr + i);
        success &= vmm_map_page(pml4, vaddr + i, paddr + i, flags);
    }
    return success;
}
_bool vmm_unmap_range(u64 pml4,u64 vaddr, u64 size){
    _bool success = true;
    for (u64 i = 0; i < size; i += PAGE_SIZE) {
        log_trace("Unmapping 0x%p\n", vaddr + i);
        success &= vmm_unmap_page(pml4, vaddr + i);
    }
    return success;
}

_bool vmm_unmap_page(u64 pml4_,u64 vaddr) {
    usize pml4_index = (vaddr >> 39) & 0x1ff;
    usize pml3_index = (vaddr >> 30) & 0x1ff;
    usize pml2_index = (vaddr >> 21) & 0x1ff;
    usize pml1_index = (vaddr >> 12) & 0x1ff;

    uptr *pml4 = (uptr *)pml4_;
    if (!(pml4[pml4_index] & X86_PAGER_BITMASK_PRESENT)) {
        return false;
    }
    uptr *pml3 = (uptr *)(pml4[pml4_index] & ~0xFFF);
    if (!(pml3[pml3_index] & X86_PAGER_BITMASK_PRESENT)) {
        return false;
    }
    uptr *pml2 = (uptr *)(pml3[pml3_index] & ~0xFFF);
    if (!(pml2[pml2_index] & X86_PAGER_BITMASK_PRESENT)) {
        return false;
    }
    uptr *pml1 = (uptr *)(pml2[pml2_index] & ~0xFFF);
    pml1[pml1_index] = 0;
    __asm__ volatile ("invlpg (%0)" : : "b"(vaddr) : "memory");
    return true;
}

u64 vmm_virt_to_phys(u64 pml4,u64 vaddr) {
    usize pml4_index = (vaddr >> 39) & 0x1ff;
    usize pml3_index = (vaddr >> 30) & 0x1ff;
    usize pml2_index = (vaddr >> 21) & 0x1ff;
    usize pml1_index = (vaddr >> 12) & 0x1ff;

    uptr *pml3 = get_next_lvl((uptr *)pml4, pml4_index, 0, true);
    uptr *pml2 = get_next_lvl(pml3, pml3_index, 0, true);
    uptr *pml1 = get_next_lvl(pml2, pml2_index, 0, true);
    return PML4_GET_ADDR(pml1[pml1_index]);
}
