#ifndef __PCI_H__
#define __PCI_H__

#include <stdint.h>
#include <stdbool.h>
/*
dataport:       32bit   = 0xCFC
commandport:    32bit   = 0xCF8
*/
struct PCIDevice{
    uint32_t portBase;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint8_t revision;
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