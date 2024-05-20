#include "MADT.h"
#include <core/kernel.h>
#include <vendor/printf.h>

void *end_addr;

u16 num_madt_entries = 0;
madt_entry *entry_ptrs[MAX_ENTRIES];

void recurse_table(void *ptr) {
    if (ptr >= end_addr || num_madt_entries > MAX_ENTRIES)
        return;
    madt_entry *e = (madt_entry *)ptr;
    entry_ptrs[num_madt_entries] = e;
    num_madt_entries++;

    recurse_table(ptr + e->h.length);
}

_bool madt_init() {

    madt_header *h = (madt_header *)find_thingy("APIC");
    data.lapic_base = h->lapic_addr;
    end_addr = (void *)h + h->h.length;

    recurse_table((void *)h + sizeof(madt_header));

    return true;
}
