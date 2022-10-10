#ifndef __PCI_H__
#define __PCI_H__

#include <util/stdint.h>
/*
dataport:       32bit   = 0xCFC
commandport:    32bit   = 0xCF8
*/

struct PCIDevice{
    uint_32 portBase;
    uint_32 interrupt;

    uint_16 bus;
    uint_16 device;
    uint_16 function;

    uint_16 vendor_id;
    uint_16 device_id;

    uint_8 class_id;
    uint_8 subclass_id;
    uint_8 interface_id;

    uint_8 revision;

    PCIDevice();
};

namespace PCI
{
    uint_32 Read(uint_16 bus, uint_16 device, uint_16 function, uint_32 offset);
    void Write(uint_16 bus, uint_16 device, uint_16 function, uint_32 offset,uint_32 value);
    bool HasFunction(uint_16 bus, uint_16 device);

    void SelectDrivers();
    PCIDevice GetDevice(uint_16 bus, uint_16 device, uint_16 function);
};

#endif // __PCI_H__