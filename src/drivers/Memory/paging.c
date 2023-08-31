#include "paging.h"
#include "PFA.h"

struct PageMapIndex index_page_map(void* addr)
{
    struct PageMapIndex result;
    uint64_t addr_num = (uint64_t)addr;

    addr_num >>= 12;
    result.P_i = addr_num & 0x1ff;

    addr_num >>= 9;
    result.PT_i = addr_num & 0x1ff;

    addr_num >>= 9;
    result.PD_i = addr_num & 0x1ff;

    addr_num >>= 9;
    result.PDP_i = addr_num & 0x1ff; 

    return result;
}

void map_memory(struct PageTable* pt,void* virt_mem,void* phys_mem)
{
    struct PageMapIndex index = index_page_map(virt_mem);
    struct PageDIREntry PDE;

    PDE = pt->entries[index.PDP_i];
    struct PageTable* PDP;
    if (!PDE.present){
        PDP = (struct PageTable*)request_page();
        memset(PDP, 0, 0x1000);
        PDE.address = (uint64_t)PDP >> 12;
        PDE.present = true;
        PDE.RW = true;
        pt->entries[index.PDP_i] = PDE;
    }
    else
    {
        PDP = (struct PageTable*)((uint64_t)PDE.address << 12);
    }
    
    
    PDE = PDP->entries[index.PD_i];
    struct PageTable* PD;
    if (!PDE.present){
        PD = (struct PageTable*)request_page();
        memset(PD, 0, 0x1000);
        PDE.address = (uint64_t)PD >> 12;
        PDE.present = true;
        PDE.RW = true;
        PDP->entries[index.PD_i] = PDE;
    }
    else
    {
        PD = (struct PageTable*)((uint64_t)PDE.address << 12);
    }

    PDE = PD->entries[index.PT_i];
    struct PageTable* PT;
    if (!PDE.present){
        PT = (struct PageTable*)request_page();
        memset(PT, 0, 0x1000);
        PDE.address = (uint64_t)PT >> 12;
        PDE.present = true;
        PDE.RW = true;
        PD->entries[index.PT_i] = PDE;
    }
    else
    {
        PT = (struct PageTable*)((uint64_t)PDE.address << 12);
    }

    PDE = PT->entries[index.P_i];
    PDE.address = (uint64_t)phys_mem >> 12;
    PDE.present = true;
    PDE.RW = true;
    PT->entries[index.P_i] = PDE;
}

struct PageTable* init_mem_and_identmap()
{
    struct PageTable* PML4 = (struct PageTable*)request_page();
    memset(PML4,0,0x1000);

    for (size_t i = 0; i < get_total_RAM(); i+= 0x1000)
    {
        map_memory(PML4,(void*)i,(void*)i);
    }
    
    asm ("mov %0, %%cr3" : : "r" (PML4));
}
