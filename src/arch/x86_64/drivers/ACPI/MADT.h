#ifndef MADT_H
#define MADT_H

#include <drivers/ACPI/RSDT.h>

#define MADT_ENTRY_TYPE_LAPIC                0
#define MADT_ENTRY_TYPE_IOAPIC               1
#define MADT_ENTRY_TYPE_IOLAPIC_OVERRIDE      2
#define MADT_ENTRY_TYPE_IOLAPIC_NMI           3
#define MADT_ENTRY_TYPE_lapic_NMI            4
#define MADT_ENTRY_TYPE_lapic_ADDR_OVERRIDE  5
#define MADT_ENTRY_TYPE_LX2APIC              9

typedef struct {
    sdt_header h;
    u32 lapic_addr;
    u32 flags;
} __attribute__((__packed__)) madt_header;

typedef struct {
    u8 type;
    u8 length;
} __attribute__((__packed__)) entry_header;

typedef struct {
    entry_header h;
    union {
        struct {
            u8 CPU_ID;
            u8 LAPIC_ID;
            u32 flags;
        } __attribute__((__packed__)) lapic;
        struct {
            u8 id;
            u8 reserved;
            u32 adress;
            u32 GS_interrupt_base;
        } __attribute__((__packed__)) ioapic;
        struct {
            u8 bus_source;
            u8 IRQ_source;
            u8 GS_interupt;
            u16 flags;
        } __attribute__((__packed__)) ioapic_override;
        struct {
            u8 source;
            u8 reserved;
            u16 flags;
            u32 GS_interrupt;
        } __attribute__((__packed__)) ioapic_nmi;
        struct {
            u8 CPU_ID;
            u16 flags;
            u8 lint_num;//?????
        } __attribute__((__packed__)) lapic_nmi;
        struct {
            u16 reserved;
            u64 addr64;
        } __attribute__((__packed__)) lapic_addr_overrride;
        struct {
            u16 reserved;
            u32 ID;
            u32 flags;
            u32 ACPI_ID;
        } __attribute__((__packed__)) lx2apic;
    } the_meat;
} __attribute__((__packed__)) madt_entry;

_bool madt_init();

#define MAX_ENTRIES 64
extern u16 num_madt_entries;
extern madt_entry* entry_ptrs[MAX_ENTRIES];

#endif
