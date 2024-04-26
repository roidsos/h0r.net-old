#include "pager.h"

uint64_t create_pagetable(){
    return 0;
}

void bind_pagetable(UNUSED uint64_t pml4){
    __asm__ volatile("mov %0, %%cr3" : : "r"(pml4));
}
uint64_t get_pagetable(){
    uint64_t pml4;
    __asm__ volatile("mov %%cr3, %0" : "=r"(pml4));
    return pml4;
}

bool map_page(UNUSED uint64_t vaddr,UNUSED uint64_t paddr,UNUSED uint64_t flags){
    return true;
}
bool unmap_page(UNUSED uint64_t vaddr){
    return true;
}

uint64_t virt_to_phys(uint64_t vaddr,uint64_t pml4){
    uint64_t pml4index = vaddr >> 22;
    uint64_t pml3index = vaddr >> 12 & 0x03FF;

    if(!(((uint64_t*)pml4)[pml4index] & BITMASK_L4_PRESENT)){
        return 0;
    }

    uint64_t *pml3 = ((uint64_t*)pml4) + (0x400 * pml4index);
    if(!(pml3[pml3index] & BITMASK_L4_PRESENT)){
        return 0;
    }

    return ((pml3[pml3index] & ~0xFFF) | ((uint64_t)vaddr & 0xFFF));
}
uint64_t phys_to_virt(UNUSED uint64_t paddr,UNUSED uint64_t pml4){
    return 0;
}
