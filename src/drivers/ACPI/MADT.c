#include "MADT.h"
#include <vendor/printf.h>
void* end_addr;

#define MAX_ENTRIES 64
uint16_t num_entries = 0;
madt_entry* entry_ptrs[MAX_ENTRIES];


void recurse_table(void* ptr){
    if(ptr >= end_addr || num_entries > MAX_ENTRIES)
        return;
    madt_entry* e = (madt_entry*)ptr;
    entry_ptrs[num_entries] = e;
    num_entries++;

    recurse_table(ptr + e->h.length);
}


bool init_madt(){

    madt_header* h =(madt_header*)find_thingy("APIC");
    end_addr = (void*)h + h->h.length;

    recurse_table((void*)h + sizeof(madt_header));

    return true;
}
