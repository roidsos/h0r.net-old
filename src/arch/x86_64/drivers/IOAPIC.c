#include "IOAPIC.h"
#include "core/mm/mem.h"
#include "vendor/printf.h"

// Sorry, I forgot to attribute this
//  this is based on:
//  https://github.com/asterd-og/ZanOS/blob/main/kernel/src/dev/ioapic.c

void *ioapicaddr = NULL;
u8 vector_count;

u32 read(u32 reg) {
    u32 volatile *ioapic = (u32 volatile *)ioapicaddr;
    ioapic[0] = (reg & 0xff);
    return ioapic[4];
}

void write(u32 reg, u32 value) {
    u32 volatile *ioapic = (u32 volatile *)ioapicaddr;
    ioapic[0] = (reg & 0xff);
    ioapic[4] = value;
}

_bool ioapic_init() {
    // TODO: do not assume HART 0
    for (usize i = 0; i < num_madt_entries; i++) {
        if (entry_ptrs[i]->h.type == MADT_ENTRY_TYPE_IOAPIC) {
            ioapicaddr =
                (void *)PHYS_TO_VIRT(entry_ptrs[i]->the_meat.ioapic.adress);
            break;
        }
    }
    if (ioapicaddr == NULL) {
        return false;
    }
    vector_count = ((read(IOLAPIC_VER) >> 16) & 0xFF);
    // mask all and set
    for (u32 i = 0; i <= vector_count; ++i) {
        write(IOLAPIC_REDTBL + 2 * i, 1 << 16 | (32 + i));
        write(IOLAPIC_REDTBL + 2 * i + 1, 0); // redir cpu
    }

    return false;
}

madt_entry *get_gsi(u32 gsi) {
    for (usize i = 0; i < num_madt_entries; i++) {
        if (entry_ptrs[i]->h.type == MADT_ENTRY_TYPE_IOAPIC &&
            entry_ptrs[i]->the_meat.ioapic.GS_interrupt_base <= gsi &&
            entry_ptrs[i]->the_meat.ioapic.GS_interrupt_base + vector_count >
                gsi) {
            return entry_ptrs[i];
        }
    }
    return NULL;
}

// TODO: dont aassume lapic ID of 0
void ioapic_mask(u8 id) {
    u8 vec = 32 + id;
    for (usize i = 0; i < num_madt_entries; i++) {
        if (entry_ptrs[i]->h.type == MADT_ENTRY_TYPE_IOLAPIC_OVERRIDE &&
            entry_ptrs[i]->the_meat.ioapic_override.IRQ_source == vec) {
            madt_entry *gsi =
                get_gsi(entry_ptrs[i]->the_meat.ioapic_override.GS_interupt);
            void *tmp = ioapicaddr;
            ioapicaddr = (void *)(u64)gsi->the_meat.ioapic.adress;

            u64 redir = id;

            if ((entry_ptrs[i]->the_meat.ioapic_override.flags & (1 << 1)) !=
                0) {
                redir |= (1 << 13);
            }
            if ((entry_ptrs[i]->the_meat.ioapic_override.flags & (1 << 3)) !=
                0) {
                redir |= (1 << 15);
            }
            write((entry_ptrs[i]->the_meat.ioapic_override.GS_interupt -
                   gsi->the_meat.ioapic.GS_interrupt_base) *
                          2 +
                      16,
                  1 << 16 | (u32)redir);
            write((entry_ptrs[i]->the_meat.ioapic_override.GS_interupt -
                   gsi->the_meat.ioapic.GS_interrupt_base) *
                          2 +
                      17,
                  (u32)(redir >> 32));
            ioapicaddr = tmp;
            return;
        }
    }
    madt_entry *gsi = get_gsi(id);
    write((id - gsi->the_meat.ioapic.GS_interrupt_base) * 2 + 16,
          1 << 16 | (vec));
    write((id - gsi->the_meat.ioapic.GS_interrupt_base) * 2 + 17,
          0); // redir cpu
}
void ioapic_unmask(u8 id) {
    u8 vec = 32 + id;
    for (usize i = 0; i < num_madt_entries; i++) {
        if (entry_ptrs[i]->h.type == MADT_ENTRY_TYPE_IOLAPIC_OVERRIDE &&
            entry_ptrs[i]->the_meat.ioapic_override.IRQ_source == vec) {
            madt_entry *gsi =
                get_gsi(entry_ptrs[i]->the_meat.ioapic_override.GS_interupt);
            void *tmp = ioapicaddr;
            ioapicaddr = (void *)(u64)gsi->the_meat.ioapic.adress;

            u64 redir = id;

            if ((entry_ptrs[i]->the_meat.ioapic_override.flags & (1 << 1)) !=
                0) {
                redir |= (1 << 13);
            }
            if ((entry_ptrs[i]->the_meat.ioapic_override.flags & (1 << 3)) !=
                0) {
                redir |= (1 << 15);
            }

            write((entry_ptrs[i]->the_meat.ioapic_override.GS_interupt -
                   gsi->the_meat.ioapic.GS_interrupt_base) *
                          2 +
                      16,
                  (u32)redir);
            write((entry_ptrs[i]->the_meat.ioapic_override.GS_interupt -
                   gsi->the_meat.ioapic.GS_interrupt_base) *
                          2 +
                      17,
                  (u32)(redir >> 32));
            ioapicaddr = tmp;
            return;
        }
    }
    madt_entry *gsi = get_gsi(id);
    void *tmp = ioapicaddr;
    ioapicaddr = (void *)(u64)gsi->the_meat.ioapic.adress;
    write((id - gsi->the_meat.ioapic.GS_interrupt_base) * 2 + 16, vec);
    write((id - gsi->the_meat.ioapic.GS_interrupt_base) * 2 + 17,
          0); // redir cpu
    ioapicaddr = tmp;
}
