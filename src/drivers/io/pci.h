#ifndef __PCI_H__
#define __PCI_H__

#include <types/stdtypes.h>

#define PCI_COMMAND 0xCF8
#define PCI_DATA 0xCFC

struct PCIDevice{
    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint32_t BAR0;
    uint32_t BAR1;
    uint32_t BAR2;
    uint32_t BAR3;
    uint32_t BAR4;
    uint32_t BAR5;

    uint8_t revision;
    uint32_t interrupt;
};


uint32_t Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset);
uint32_t Read_(struct PCIDevice self, uint32_t offset);
void Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset,uint32_t value);
void Write_(struct PCIDevice self,uint32_t offset,uint32_t value);

bool HasFunction(uint16_t bus, uint16_t device);

void init_PCI();
void list_PCI_devices();
struct PCIDevice GetDevice(uint16_t bus, uint16_t device, uint16_t function);

#endif // __PCI_H__