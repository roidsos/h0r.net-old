#include "IOAPIC.h"
#include "core/memory.h"
#include "vendor/printf.h"

void *ioapicaddr = NULL;
uint8_t vector_count;

uint32_t read(uint32_t reg) {
    uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
    ioapic[0] = (reg & 0xff);
    return ioapic[4];
}

void write(uint32_t reg, uint32_t value) {
    uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
    ioapic[0] = (reg & 0xff);
    ioapic[4] = value;
}

bool init_ioapic() {
    // TODO: do not assume HART 0
    for (size_t i = 0; i < num_madt_entries; i++) {
        if (entry_ptrs[i]->h.type == MADT_ENTRY_TYPE_IOAPIC) {
            ioapicaddr =
                (void *)PHYS_TO_VIRT(entry_ptrs[i]->the_meat.ioapic.adress);
            break;
        }
    }
    if (ioapicaddr == NULL) {
        return false;
    }
    vector_count = ((read(IOAPIC_VER) >> 16) & 0xFF);
    // mask all and set
    for (uint32_t i = 0; i <= vector_count; ++i) {
        write(IOAPIC_REDTBL + 2 * i, 1 << 16 | (32 + i));
        write(IOAPIC_REDTBL + 2 * i + 1, 0); // redir cpu
    }

    return false;
}

madt_entry *get_gsi(uint32_t gsi) {
    for (size_t i = 0; i < num_madt_entries; i++) {
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
void mask(uint8_t id) {
    uint8_t vec = 32 + id;
    for (size_t i = 0; i < num_madt_entries; i++) {
        if (entry_ptrs[i]->h.type == MADT_ENTRY_TYPE_IOAPIC_OVERRIDE &&
            entry_ptrs[i]->the_meat.ioapic_override.IRQ_source == vec) {
            madt_entry *gsi =
                get_gsi(entry_ptrs[i]->the_meat.ioapic_override.GS_interupt);
            void *tmp = ioapicaddr;
            ioapicaddr = (void *)(uint64_t)gsi->the_meat.ioapic.adress;

            uint64_t redir = id;

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
                  1 << 16 | (uint32_t)redir);
            write((entry_ptrs[i]->the_meat.ioapic_override.GS_interupt -
                   gsi->the_meat.ioapic.GS_interrupt_base) *
                          2 +
                      17,
                  (uint32_t)(redir >> 32));
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
void unmask(uint8_t id) {
    uint8_t vec = 32 + id;
    for (size_t i = 0; i < num_madt_entries; i++) {
        if (entry_ptrs[i]->h.type == MADT_ENTRY_TYPE_IOAPIC_OVERRIDE &&
            entry_ptrs[i]->the_meat.ioapic_override.IRQ_source == vec) {
            madt_entry *gsi =
                get_gsi(entry_ptrs[i]->the_meat.ioapic_override.GS_interupt);
            void *tmp = ioapicaddr;
            ioapicaddr = (void *)(uint64_t)gsi->the_meat.ioapic.adress;

            uint64_t redir = id;

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
                  (uint32_t)redir);
            write((entry_ptrs[i]->the_meat.ioapic_override.GS_interupt -
                   gsi->the_meat.ioapic.GS_interrupt_base) *
                          2 +
                      17,
                  (uint32_t)(redir >> 32));
            ioapicaddr = tmp;
            return;
        }
    }
    madt_entry *gsi = get_gsi(id);
    void *tmp = ioapicaddr;
    ioapicaddr = (void *)(uint64_t)gsi->the_meat.ioapic.adress;
    write((id - gsi->the_meat.ioapic.GS_interrupt_base) * 2 + 16, vec);
    write((id - gsi->the_meat.ioapic.GS_interrupt_base) * 2 + 17,
          0); // redir cpu
    ioapicaddr = tmp;
}
