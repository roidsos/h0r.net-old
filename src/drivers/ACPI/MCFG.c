#include "MCFG.h"
#include <vendor/printf.h>

// Puffer get outta here, stop copying my code ffs

uint32_t num_mcfg_entries;
device_config *entries;

bool init_mcfg() {
    mcfg_header *h = (mcfg_header *)find_thingy("MCFG");
    if (h == NULL) {
        return false;
    }
    num_mcfg_entries =
        (h->h.length - sizeof(mcfg_header)) / sizeof(device_config);
    entries = (device_config *)((uint8_t *)h + sizeof(mcfg_header));
    return true;
}
void iterate_pci() {
    printf("PCI Devices: \n");
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs = pci_read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                uint16_t vendor_id = pci_read(bus, device, function, 0x00);
                uint16_t device_id = pci_read(bus, device, function, 0x02);
                if (vendor_id == 0x0000 || vendor_id == 0xFFFF) {
                    continue;
                }
                printf("VendorID: 0x%x,DeviceID: 0x%x \n", vendor_id,
                       device_id);
            }
        }
    }
}

uint32_t *pci_getaddr(uint8_t bus, uint8_t dev, uint8_t func, uint8_t off) {
    for (size_t i = 0; i < num_mcfg_entries; i++) {
        if (bus >= entries[i].start_bus && bus <= entries[i].end_bus) {
            uint64_t addr = (entries[i].base_address + (bus << 20) +
                             (dev << 15) + (func << 12) + off);
            return (uint32_t *)addr;
        }
    }
    return 0;
}

uint32_t pci_read(uint8_t bus, uint8_t dev, uint8_t func, uint8_t off) {

    return pci_getaddr(bus, dev, func, off)[0];
}
void pci_write(uint8_t bus, uint8_t dev, uint8_t func, uint8_t off,
               uint32_t val) {
    pci_getaddr(bus, dev, func, off)[0] = val;
}
