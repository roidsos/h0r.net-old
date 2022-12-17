#include <drivers/pci.h>
#include <io/io.h>
#include <lib/printf.h>
namespace PCI{
uint_32 Read(uint_16 bus,uint_16 device,uint_16 function,uint_32 offset)
{
    uint_32 id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (offset & 0xFC);
    outb32(0xCF8,id);
    register uint_32 result = inb32(0xCFC + (offset & 3));
    return result;
}

void Write(uint_16 bus, uint_16 device, uint_16 function, uint_32 offset,uint_32 value)
{
    
    uint_32 id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (offset & 0xFC);
    outb32(0xCF8,id);
    outb32(0xCFC,value);
}

bool HasFunction(uint_16 bus, uint_16 device)
{
    return Read(bus,device,0,0x0E) & (1<<7);
}

void SelectDrivers()
{
    printf("PCI Devices: \n");
    for(int bus = 0;bus < 8;bus++)
    {
        for(int device = 0;device < 32;device++)
        {
            int numfuncs = HasFunction(bus,device) ? 8 : 1;
            for(int function = 0;function < numfuncs;function++)
            {
                PCIDevice dev  =  GetDevice(bus,device,function);

                if(dev.vendor_id == 0x0000 ||dev.vendor_id == 0xFFFF){
                    //printf("nothing\n");
                    break;
                }
                printf("VendorID: %i ,",dev.vendor_id);
                printf("DeviceID: %i \n",dev.device_id);
            }
            
        }   
    }
}

PCIDevice GetDevice(uint_16 bus, uint_16 device, uint_16 function)
{
    PCIDevice result;
    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus,device,function,0x00);
    result.device_id = Read(bus,device,function,0x02);

    result.class_id = Read(bus,device,function,0x0B);
    result.subclass_id = Read(bus,device,function,0x0A);
    result.interface_id = Read(bus,device,function,0x09);

    result.revision = Read(bus,device,function,0x08);
    result.interrupt = Read(bus,device,function,0x3c);
}



}

PCIDevice::PCIDevice()
{
    
}
