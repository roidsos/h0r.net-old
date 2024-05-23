#ifdef __x86_64__ // Since GDT is x86-only, we use a preprocessor condition to
                  // know if we need to compile the GDT.

#include "gdt.h"

gdt_entry gdt[0xFF];
gdt_pointer gdtr;
tss_entry_t tss_entry;

extern void load_gdt(gdt_pointer *);
extern void load_tss();

void set_gdt_entry(int i, u16 limit, u16 base, u8 access, u8 flags) {
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = access;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}
void set_tss_gate(int i, u64 base, u32 limit) {
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = 0xE9;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | 0x00;
}

void init_tss(u64 rsp0) {
    char *ptr_c = (char *)&tss_entry;
    for (usize i = 0; i < sizeof(tss_entry_t); i++) {
        ptr_c[i] = (char)0;
    }
    tss_entry.rsp0 = rsp0;
}

int gdt_init(u64 *rsp0) {
    set_gdt_entry(0, 0, 0, 0, 0);             // NULL segment
    set_gdt_entry(1, 0, 0, 0b10011010, 0xA0); // Ring 0 code
    set_gdt_entry(2, 0, 0, 0b10010010, 0xA0); // Ring 0 data
    set_gdt_entry(3, 0, 0, 0b10111010, 0xA0); // Ring 1 code
    set_gdt_entry(4, 0, 0, 0b10110010, 0xA0); // Ring 1 data
    set_gdt_entry(5, 0, 0, 0b11011010, 0xA0); // Ring 2 code
    set_gdt_entry(6, 0, 0, 0b11010010, 0xA0); // Ring 2 data
    set_gdt_entry(7, 0, 0, 0b11111010, 0xA0); // Ring 3 code
    set_gdt_entry(8, 0, 0, 0b11110010, 0xA0); // Ring 3 data
    set_tss_gate(9, (u64)&tss_entry, sizeof(tss_entry_t));

    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (u64)&gdt;

    init_tss((u64)rsp0);
    load_gdt(&gdtr);
    load_tss();

    return 0;
}

void set_kernel_stack(u64 *stack) { tss_entry.rsp0 = (u64)stack; }

#endif