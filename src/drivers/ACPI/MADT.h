#ifndef MADT_H
#define MADT_H

#include "RSDT.h"

#define MADT_ENTRY_TYPE_LAPIC                0
#define MADT_ENTRY_TYPE_IOAPIC               1
#define MADT_ENTRY_TYPE_IOAPIC_OVERRIDE      2
#define MADT_ENTRY_TYPE_IOAPIC_NMI           3
#define MADT_ENTRY_TYPE_LAPIC_NMI            4
#define MADT_ENTRY_TYPE_LAPIC_ADDR_OVERRIDE  5
#define MADT_ENTRY_TYPE_LX2APIC              9

typedef struct {
    sdt_header h;
    uint32_t lapic_addr;
    uint32_t flags;
} __attribute__((__packed__)) madt_header;

typedef struct {
    uint8_t type;
    uint8_t length;
} __attribute__((__packed__)) entry_header;

typedef struct {
    entry_header h;
    union {
        struct {
            uint8_t CPU_ID;
            uint8_t APIC_ID;
            uint32_t flags;
        } __attribute__((__packed__)) lapic;
        struct {
            uint8_t id;
            uint8_t reserved;
            uint32_t adress;
            uint32_t GS_interrupt_base;
        } __attribute__((__packed__)) ioapic;
        struct {
            uint8_t bus_source;
            uint8_t IRQ_source;
            uint8_t GS_interupt;
            uint16_t flags;
        } __attribute__((__packed__)) ioapic_override;
        struct {
            uint8_t source;
            uint8_t reserved;
            uint16_t flags;
            uint32_t GS_interrupt;
        } __attribute__((__packed__)) ioapic_nmi;
        struct {
            uint8_t CPU_ID;
            uint16_t flags;
            uint8_t lint_num;//?????
        } __attribute__((__packed__)) lapic_nmi;
        struct {
            uint16_t reserved;
            uint64_t addr64;
        } __attribute__((__packed__)) lapic_addr_overrride;
        struct {
            uint16_t reserved;
            uint32_t ID;
            uint32_t flags;
            uint32_t ACPI_ID;
        } __attribute__((__packed__)) lx2apic;
    } the_meat;
} __attribute__((__packed__)) madt_entry;

bool madt_init();

#define MAX_ENTRIES 64
extern uint16_t num_madt_entries;
extern madt_entry* entry_ptrs[MAX_ENTRIES];

#endif
