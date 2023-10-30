#ifdef __x86_64__ // Since GDT is x86-only, we use a preprocessor condition to
                  // know if we need to compile the GDT.

#include "gdt.h"

gdt_entry gdt[0xFF];
gdt_pointer gdtr;
tss_entry_t tss_entry;

extern void load_gdt(gdt_pointer *);
extern void load_tss();

void set_gdt_entry(int i, uint16_t limit, uint16_t base, uint8_t access,
                   uint8_t flags) {
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = access;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}
void set_tss_gate(int i, uint64_t base, uint32_t limit) {
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = 0xE9;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | 0x00;
}

void init_tss(uint64_t rsp0) {
    char *ptr_c = (char *)&tss_entry;
    for (size_t i = 0; i < sizeof(tss_entry_t); i++) {
        ptr_c[i] = (char)0;
    }
    tss_entry.rsp0 = rsp0;
}

int gdt_init(uint64_t *rsp0) {
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0, 0, 0b10011010, 0xA0);
    set_gdt_entry(2, 0, 0, 0b10010010, 0xA0);
    set_gdt_entry(3, 0, 0, 0b10111010, 0xA0);
    set_gdt_entry(4, 0, 0, 0b10110010, 0xA0);
    set_gdt_entry(5, 0, 0, 0b11011010, 0xA0);
    set_gdt_entry(6, 0, 0, 0b11010010, 0xA0);
    set_gdt_entry(7, 0, 0, 0b11111010, 0xA0);
    set_gdt_entry(8, 0, 0, 0b11110010, 0xA0);
    set_tss_gate(9, (uint64_t)&tss_entry, sizeof(tss_entry_t));

    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (uint64_t)&gdt;

    init_tss((uint64_t)rsp0);
    load_gdt(&gdtr);
    load_tss();

    return 0;
}

void set_kernel_stack(uint64_t *stack) { tss_entry.rsp0 = (uint64_t)stack; }

#endif