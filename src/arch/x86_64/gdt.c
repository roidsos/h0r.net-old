#ifdef __x86_64__ // Since GDT is x86-only, we use a preprocessor condition to
                  // know if we need to compile the GDT.

#include "gdt.h"

gdt_table gdt;
gdt_pointer gdtr;
tss daTSS;

extern void load_gdt(gdt_pointer *);
extern void load_tss();

void set_gdt_entry(int i, u16 limit, u16 base, u8 access, u8 flags) {
    gdt.gdt_entries[i].base0 = base & 0xFFFF;
    gdt.gdt_entries[i].base1 = (base >> 16) & 0xFF;
    gdt.gdt_entries[i].base2 = (base >> 24) & 0xFF;
    gdt.gdt_entries[i].limit0 = limit & 0xFFFF;
    gdt.gdt_entries[i].access = access;
    gdt.gdt_entries[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}
void set_tss_gate(u64 tss) {
    gdt.tss_entry.length = sizeof(tss);
    gdt.tss_entry.base = (u16)(tss & 0xffff);
    gdt.tss_entry.base1 = (u8)((tss >> 16) & 0xff);
    gdt.tss_entry.flags = 0x89;
    gdt.tss_entry.flags1 = 0;
    gdt.tss_entry.base2 = (u8)((tss >> 24) & 0xff);
    gdt.tss_entry.base3 = (u32)(tss >> 32);
    gdt.tss_entry.resv = 0;
}

void init_tss(u64 rsp0) {
    char *ptr_c = (char *)&daTSS;
    for (usize i = 0; i < sizeof(tss); i++) {
        ptr_c[i] = (char)0;
    }
    daTSS.rsp0 = rsp0;
}

int gdt_init(u64 *rsp0) {
    set_gdt_entry(0, 0, 0, 0, 0);             // NULL segment
    set_gdt_entry(1, 0, 0, 0b10011010, 0xA0); // Ring 0 code
    set_gdt_entry(2, 0, 0, 0b10010010, 0xC0); // Ring 0 data
    set_gdt_entry(3, 0, 0, 0b10111010, 0xA0); // Ring 1 code
    set_gdt_entry(4, 0, 0, 0b10110010, 0xC0); // Ring 1 data
    set_gdt_entry(5, 0, 0, 0b11011010, 0xA0); // Ring 2 code
    set_gdt_entry(6, 0, 0, 0b11010010, 0xC0); // Ring 2 data
    set_gdt_entry(7, 0, 0, 0b11111010, 0xA0); // Ring 3 code
    set_gdt_entry(8, 0, 0, 0b11110010, 0xC0); // Ring 3 data
    set_tss_gate((u64)&daTSS);

    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (u64)&gdt;

    init_tss((u64)rsp0);
    load_gdt(&gdtr);
    load_tss();

    return 0;
}

void set_kernel_stack(u64 *stack) { daTSS.rsp0 = (u64)stack; }

#endif