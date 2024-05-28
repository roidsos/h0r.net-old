#ifndef __PCI_H__
#define __PCI_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>

typedef struct {
    u32 (*read)(u8 bus, u8 dev, u8 func, u8 off);
    void (*write)(u8 bus, u8 dev, u8 func, u8 off, u32 val);
} pci_aspace_t;

typedef struct {
    u8 bus;
    u8 dev;
    u8 func;
} pci_dev_addr_t;

typedef struct {
    pci_dev_addr_t* addrs;
    u8 count;
} pci_multi_dev_t;

typedef struct {
    pci_dev_addr_t addr;

    u16 vendor_id;
    u16 device_id;

    u8 class_base;
    u8 class_sub;
    u8 prog_if;

    u8 capabilities;
    u8 interrupt_line;
    u8 interrupt_pin;

    u32 bar0;
    u32 bar1;
    u32 bar2;
    u32 bar3;
    u32 bar4;
    u32 bar5;
    u32 expansion_rom;
    u32 cardbus_cis;

    u16 subsystem_id;
    u16 subsystem_vendor_id;
    u8 revision_id;

    u8 selftest_status;
    u8 min_grant;
    u8 max_latency;
} pci_dev_info_t;

// this will just overwrite the current aspace, since we only need one
void register_aspace(pci_aspace_t aspace);

// traverse the latest aspace.
_bool pci_init(void);

pci_multi_dev_t pci_find_devices_by_id(u16 vendor_id, u16 device_id);
pci_multi_dev_t pci_find_devices_by_class(u8 class_base, u8 class_sub);

pci_dev_info_t pci_get_device_info(pci_dev_addr_t addr);
#endif // __PCI_H__