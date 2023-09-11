#include "pci.h"
#include <io/portio.h>
#include <logging/logger.h>
#include <vendor/printf.h>

struct PCIDevice devices[2048];
bool pci_initialized;

uint32_t Read(uint16_t bus, uint16_t device, uint16_t function,
              uint32_t offset) {
    uint32_t id =
        0x1 << 31                  // first bit is 1
        | ((bus & 0xFF) << 16)     // bits 2-15 are the bus
        | ((device & 0x1F) << 11)  // bits 16-21 are the device
        | ((function & 0x07) << 8) // bits 22-24 are the function
        |
        (offset & 0xFC); // the remaining bits are the offset, so every function
                         // gets 255 bytes of registers minus the header
    outb32(PCI_COMMAND, id);
    register uint32_t result = inb32(PCI_DATA);
    return result >> (8 * (offset % 4));
}

void Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset,
           uint32_t value) {

    uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) |
                  ((function & 0x07) << 8) | (offset & 0xFC);
    outb32(PCI_COMMAND, id);
    outb32(PCI_DATA, value);
}

bool HasFunction(uint16_t bus, uint16_t device) {
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void init_PCI() {
    // https://wiki.osdev.org/PCI
    log_info("PCI Devices: ");
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs = HasFunction(bus, device) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                struct PCIDevice dev = GetDevice(bus, device, function);
                devices[(bus * 8) + (device * 32) + function] = dev;

                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) {
                    // printf_("nothing\n");
                    continue;
                }

                log_info("VendorID: 0x%x ,DeviceID: 0x%x", dev.vendor_id,
                         dev.device_id);
            }
        }
    }
    pci_initialized = true;
}
void list_PCI_devices() {
    printf_("PCI Devices: \n");
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs = HasFunction(bus, device) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                struct PCIDevice dev = GetDevice(bus, device, function);
                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) {
                    continue;
                }
                printf_("VendorID: 0x%x,DeviceID: 0x%x \n", dev.vendor_id,
                        dev.device_id);
            }
        }
    }
}

struct PCIDevice GetDevice(uint16_t bus, uint16_t device, uint16_t function) {
    if (pci_initialized) {
        return devices[(bus * 8) + (device * 32) + function];
    }

    struct PCIDevice result;
    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0B) & 0x000000FF;
    result.subclass_id = Read(bus, device, function, 0x0A) & 0x000000FF;
    result.interface_id = Read(bus, device, function, 0x09) & 0x000000FF;

    result.revision = Read(bus, device, function, 0x08) & 0x000000FF;
    result.interrupt = Read(bus, device, function, 0x3c) & 0x000000FF;

    result.BAR0 = Read(bus, device, function, 0x10);
    result.BAR1 = Read(bus, device, function, 0x14);
    result.BAR2 = Read(bus, device, function, 0x18);
    result.BAR3 = Read(bus, device, function, 0x1C);
    result.BAR4 = Read(bus, device, function, 0x20);
    result.BAR5 = Read(bus, device, function, 0x24);

    return result;
}

void Write_(struct PCIDevice self, uint32_t offset, uint32_t value) {
    Write(self.bus, self.device, self.function, offset, value);
}

uint32_t Read_(struct PCIDevice self, uint32_t offset) {
    return Read(self.bus, self.device, self.function, offset);
}