#include "PCI.h"
#include <libk/stddef.h>
#include <utils/log.h>
pci_aspace_t pci_aspace = {
    .read = NULL,
    .write = NULL
};

void iterate_pci() {
    log_trace("PCI Devices: \n");
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs = pci_aspace.read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                u16 vendor_id = pci_aspace.read(bus, device, function, 0x00);
                u16 device_id = pci_aspace.read(bus, device, function, 0x02);
                u8 class_base = pci_aspace.read(bus, device, function, 0x0B);
                u8 class_sub = pci_aspace.read(bus, device, function, 0x0A);
                u8 prog_if = pci_aspace.read(bus, device, function, 0x09);
                u8 header_type = pci_aspace.read(bus, device, function, 0x0E) & 0x7F;
                if (device_id == 0x0000 || vendor_id == 0xFFFF) {
                    continue;
                }
                log_trace("  -VendorID: 0x%x,DeviceID: 0x%x,Class: 0x%x,Subclass: 0x%x,ProgIF: 0x%x,HeaderType: 0x%x \n", vendor_id,
                          device_id, class_base, class_sub, prog_if, header_type);
            }
        }
    }
}

void register_aspace(pci_aspace_t aspace)
{
    pci_aspace = aspace;
}

_bool pci_init(void)
{
    if(pci_aspace.read == NULL || pci_aspace.write == NULL)
        return false;

    iterate_pci();

    return true;
}