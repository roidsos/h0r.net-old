#ifndef MCFG_H
#define MCFG_H

#include "RSDT.h"

typedef struct{
    sdt_header h;
    uint64_t reserved;
}__attribute__((packed)) mcfg_header;

typedef struct{
    uint64_t base_address;
    uint16_t PCI_seg_group;
    uint8_t start_bus;
    uint8_t end_bus;
    uint32_t reserved;
}__attribute__((packed)) device_config;

bool mcfg_init();
void iterate_pci();

uint32_t* pci_getaddr(uint8_t bus,uint8_t dev,uint8_t func,uint8_t off);

uint32_t pci_read(uint8_t bus,uint8_t dev,uint8_t func,uint8_t off);
void pci_write(uint8_t bus,uint8_t dev,uint8_t func,uint8_t off,uint32_t val);

#endif
