#include "scubadeeznutz.h"
#include <arch/x86/interrupts/interrupts.h>
#include <core/Memory/PFA.h>
#include <klibc/memory.h>

// "hippity hoppity,SCUBA DEEZ NUTZ"
// https://github.com/luxeos/luxe-kernel-old/

void scuba_map(uint64_t *_pml4, uint64_t virt_addr, uint64_t phys_addr,
               uint64_t np, uint64_t flags) {

    for (size_t i = 0; i < np * BLOCK_SIZE; i += BLOCK_SIZE) {
        _scuba_map(_pml4, virt_addr + i, phys_addr + i, flags);
    }
}

void scuba_unmap(uint64_t *_pml4, uint64_t virt_addr, uint64_t np) {

    for (size_t i = 0; i < np * BLOCK_SIZE; i += BLOCK_SIZE) {
        _scuba_unmap(_pml4, virt_addr + i);
    }
}

void _scuba_map(uint64_t *_pml4, uint64_t virt_addr, uint64_t phys_addr,
                uint64_t flags) {
    uint16_t pml4e = (virt_addr >> 39) & 0x1ff;
    uint64_t *pdpt = (uint64_t *)PHYS_TO_VIRT(_pml4[pml4e] & ~(0xfff));
    if (!(_pml4[pml4e] & VIRT_FLAG_PRESENT)) {
        pdpt = (uint64_t *)PHYS_TO_VIRT(request_pages(8));
        memset(pdpt, 0, 8 * BLOCK_SIZE);
        _pml4[pml4e] = ((VIRT_TO_PHYS(pdpt) & ~(0xfff)) | VIRT_FLAGS_USERMODE);
    }

    uint16_t pdpe = (virt_addr >> 30) & 0x1ff;
    uint64_t *pd = (uint64_t *)PHYS_TO_VIRT(pdpt[pdpe] & ~(0xfff));
    if (!(pdpt[pdpe] & VIRT_FLAG_PRESENT)) {
        pd = (uint64_t *)PHYS_TO_VIRT(request_pages(8));
        memset(pd, 0, 8 * BLOCK_SIZE);
        pdpt[pdpe] = ((VIRT_TO_PHYS(pd) & ~(0xfff)) | VIRT_FLAGS_USERMODE);
    }

    uint16_t pde = (virt_addr >> 21) & 0x1ff;
    uint64_t *pt = (uint64_t *)PHYS_TO_VIRT(pd[pde] & ~(0xfff));
    if (!(pd[pde] & VIRT_FLAG_PRESENT)) {
        pt = (uint64_t *)PHYS_TO_VIRT(request_pages(8));
        memset(pt, 0, 8 * BLOCK_SIZE);
        pd[pde] = ((VIRT_TO_PHYS(pt) & ~(0xfff)) | VIRT_FLAGS_USERMODE);
    }

    uint16_t pte = (virt_addr >> 12) & 0x1ff;
    pt[pte] = (phys_addr | flags);

    uint64_t cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
    if (cr3 == (uint64_t)(VIRT_TO_PHYS(_pml4))) {
        __asm__ volatile("invlpg (%0)" ::"b"((void *)virt_addr) : "memory");
    }
}

void _scuba_unmap(uint64_t *_pml4, uint64_t virt_addr) {

    uint16_t pml4e = (virt_addr >> 39) & 0x1ff;
    if (!(_pml4[pml4e] & VIRT_FLAG_PRESENT)) {
        return;
    }

    uint64_t *pdpt = (uint64_t *)PHYS_TO_VIRT(_pml4[pml4e] & ~(0x1ff));
    uint16_t pdpe = (virt_addr >> 30) & 0x1ff;
    if (!(pdpt[pdpe] & VIRT_FLAG_PRESENT)) {
        return;
    }

    uint64_t *pd = (uint64_t *)PHYS_TO_VIRT(pdpt[pdpe] & ~(0x1ff));
    uint16_t pde = (virt_addr >> 21) & 0x1ff;
    if (!(pd[pde] & VIRT_FLAG_PRESENT)) {
        return;
    }

    uint64_t *pt = (uint64_t *)PHYS_TO_VIRT(pd[pde] & ~(0x1ff));
    uint16_t pte = (virt_addr >> 12) & 0x1ff;
    if (!(pt[pte] & VIRT_FLAG_PRESENT)) {
        return;
    }

    pt[pte] = 0;
    uint64_t cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
    if (cr3 == (uint64_t)(VIRT_TO_PHYS(_pml4))) {
        __asm__ volatile("invlpg (%0)" ::"b"((void *)virt_addr) : "memory");
    }

    for (int i = 0; i < 512 * 8; i++) {
        if (pt[i] != 0) {
            return;
        }
    }

    pd[pde] = 0;
    free_pages((void *)VIRT_TO_PHYS(pt), 8);

    for (int i = 0; i < 512 * 8; i++) {
        if (pd[i] != 0) {
            return;
        }
    }

    pdpt[pdpe] = 0;
    free_pages((void *)VIRT_TO_PHYS(pd), 8);

    for (int i = 0; i < 512 * 8; i++) {
        if (pdpt[i] != 0) {
            return;
        }
    }

    _pml4[pml4e] = 0;
    free_pages((void *)VIRT_TO_PHYS(pdpt), 8);
}
void *create_pml4() {
    void *pml4;
    pml4 = (void*)VIRT_TO_PHYS(request_pages(8));
    memset(pml4, 0, 8 * BLOCK_SIZE);

    return pml4;
}
