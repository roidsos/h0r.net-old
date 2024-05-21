#ifndef MCFG_H
#define MCFG_H

#include "SDT.h"

typedef struct{
    sdt_header h;
    u64 reserved;
}__attribute__((packed)) mcfg_header;

typedef struct{
    u64 base_address;
    u16 PCI_seg_group;
    u8 start_bus;
    u8 end_bus;
    u32 reserved;
}__attribute__((packed)) device_config;

_bool mcfg_init();
void iterate_pci();

u32* pci_getaddr(u8 bus,u8 dev,u8 func,u8 off);

u32 pci_read(u8 bus,u8 dev,u8 func,u8 off);
void pci_write(u8 bus,u8 dev,u8 func,u8 off,u32 val);

#endif
