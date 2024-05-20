#ifndef HPET_H
#define HPET_H

#include  "RSDT.h"

typedef struct {
    sdt_header header;
    u8 hardware_rev_id;
    u8 info;
    u16 pci_id;
    u8 address_space_id;
    u8 register_width;
    u8 register_offset;
    u8 reserved;
    u64 addr;
    u8 hpet_num;
    u16 minimum_ticks;
    u8 page_protection;
} __attribute__((packed)) hpet_header;

typedef struct {
    u64 capabilities;
    u64 reserved0;
    u64 general_config;
    u64 reserved1;
    u64 int_status;
    u64 reserved2;
    u64 reserved3[24];
    volatile u64 counter_val;
    u64 unused4;
} __attribute__((packed)) hpet_regs;

_bool hpet_init();
void hpet_usleep(u64 us);

#endif
