#include "MCFG.h"
#include <utils/log.h>
#include <vendor/printf.h>

// Puffer get outta here, stop copying my code ffs

u32 num_mcfg_entries;
device_config *entries;

_bool mcfg_init() {
    mcfg_header *h = (mcfg_header *)find_thingy("MCFG");
    if (h == NULL) {
        return false;
    }
    num_mcfg_entries =
        (h->h.length - sizeof(mcfg_header)) / sizeof(device_config);
    entries = (device_config *)((u8 *)h + sizeof(mcfg_header));
    return true;
}
void iterate_pci() {
    log_trace("PCI Devices: \n");
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs = pci_read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                u16 vendor_id = pci_read(bus, device, function, 0x00);
                u16 device_id = pci_read(bus, device, function, 0x02);
                if (vendor_id == 0x0000 || vendor_id == 0xFFFF ||
                    device_id == 0x0000) {
                    continue;
                }
                log_trace("  -VendorID: 0x%x,DeviceID: 0x%x \n", vendor_id,
                          device_id);
            }
        }
    }
}

u32 *pci_getaddr(u8 bus, u8 dev, u8 func, u8 off) {
    for (usize i = 0; i < num_mcfg_entries; i++) {
        if (bus >= entries[i].start_bus && bus <= entries[i].end_bus) {
            u64 addr = (entries[i].base_address + (bus << 20) + (dev << 15) +
                        (func << 12) + off);
            return (u32 *)addr;
        }
    }
    return 0;
}

u32 pci_read(u8 bus, u8 dev, u8 func, u8 off) {

    return pci_getaddr(bus, dev, func, off)[0];
}
void pci_write(u8 bus, u8 dev, u8 func, u8 off, u32 val) {
    pci_getaddr(bus, dev, func, off)[0] = val;
}
