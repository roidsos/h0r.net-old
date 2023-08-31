#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>
#include <stdbool.h>

struct PageDIREntry
{
    bool present : 1;
    bool RW : 1;
    bool user_super : 1;
    bool write_through : 1;
    bool no_cash : 1;// :(
    bool access : 1; 
    bool ignore0 : 1; 
    bool beeg_pages : 1; 
    bool ignore1 : 1; 
    uint8_t available : 3; 
    uint64_t address : 52; 
}__attribute__((packed));
struct PageTable{
    struct PageDIREntry entries[512];
}__attribute__((aligned(0x1000)));

struct PageMapIndex
{
    uint64_t PDP_i;
    uint64_t PD_i;
    uint64_t PT_i;
    uint64_t P_i;
};
struct PageTable* init_mem_and_identmap();
struct PageMapIndex index_page_map(void* addr);
void map_memory(struct PageTable* pt,void* virt_mem,void* phys_mem);

#endif // __PAGING_H__