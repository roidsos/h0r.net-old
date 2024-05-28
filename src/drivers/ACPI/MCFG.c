#include "MCFG.h"
#include <vendor/printf.h>
#include <drivers/meta/PCI.h>
// Puffer get outta here, stop copying my code ffs
u32 num_mcfg_entries;
device_config *entries;

u32 *mcfg_getaddr(u8 bus, u8 dev, u8 func, u8 off) {
    for (usize i = 0; i < num_mcfg_entries; i++) {
        if (bus >= entries[i].start_bus && bus <= entries[i].end_bus) {
            u64 addr = (entries[i].base_address + (bus << 20) + (dev << 15) +
                        (func << 12) + off);
            return (u32 *)addr;
        }
    }
    return 0;
}

u32 mfg_read(u8 bus, u8 dev, u8 func, u8 off) {

    return mcfg_getaddr(bus, dev, func, off)[0];
}
void mcfg_write(u8 bus, u8 dev, u8 func, u8 off, u32 val) {
    mcfg_getaddr(bus, dev, func, off)[0] = val;
}

_bool mcfg_init() {
    mcfg_header *h = (mcfg_header *)find_SDT("MCFG");
    if (h == NULL) {
        return false;
    }
    num_mcfg_entries =
        (h->h.length - sizeof(mcfg_header)) / sizeof(device_config);
    entries = (device_config *)((u8 *)h + sizeof(mcfg_header));

    pci_aspace_t mcfg_aspace = {
        .read = mfg_read,
        .write = mcfg_write
    };
    register_aspace(mcfg_aspace);

    return true;
}

