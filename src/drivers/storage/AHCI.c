#include "AHCI.h"
#include "core/mm/mem.h"
#include "utils/log.h"
#include <drivers/meta/PCI.h>
#include <libk/endian.h>

HBA_mem_t *hba_mem;

_bool ahci_init() {
    pci_multi_dev_t devs = pci_find_devices_by_class(0x1, 0x6);

    if (devs.count == 0) {
        log_error("AHCI: No devices found\n");
        return false;
    }
    // TODO: check for multiple AHCI controllers
    hba_mem = (HBA_mem_t *)PHYS_TO_VIRT(pci_get_bar5(devs.addrs[0]).addr);
    log_trace("Found AHCI v%d controller!!\n", be32toh(hba_mem->vs));

    return true;
}
