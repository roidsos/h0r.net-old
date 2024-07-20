#ifndef __PCI_H__
#define __PCI_H__

#include <core/libk/stdint.h>
#include <core/libk/stdbool.h>

#define PCI_REG_COMMAND_IO_SPACE                    1 << 0
#define PCI_REG_COMMAND_MEM_SPACE                   1 << 1
#define PCI_REG_COMMAND_MASTER                      1 << 2
#define PCI_REG_COMMAND_SPECIAL_CYCLE               1 << 3 //Note: doesnt apply to PCIe
#define PCI_REG_COMMAND_MWIE                        1 << 4 //Note: doesnt apply to PCIe
#define PCI_REG_COMMAND_VGA_PALETTE_SNOOP           1 << 5 //Note: doesnt apply to PCIe
#define PCI_REG_COMMAND_ERROR_RESPONSE              1 << 6
#define PCI_REG_COMMAND_RESERVED                    1 << 7 
#define PCI_REG_COMMAND_SERR                        1 << 8
#define PCI_REG_COMMAND_FASTB2B                     1 << 9 //Note: doesnt apply to PCIe
#define PCI_REG_COMMAND_INTX_DISABLE                1 << 10

#define PCI_REG_STATUS_INTERRUPT_STATUS             1 << 3
#define PCI_REG_STATUS_CAPABILITY_LIST              1 << 4 //Note: mandatory on PCIe
#define PCI_REG_STATUS_66MHZ_CAPABLE                1 << 5 //Note: doesnt apply to PCIe
#define PCI_REG_STATUS_FASTB2B_CAPABLE              1 << 7 //Note: doesnt apply to PCIe
#define PCI_REG_STATUS_MASTER_DATA_PARITY_ERROR     1 << 8
#define PCI_REG_STATUS_DEVSEL_TIMING                3 << 9 //Note: doesnt apply to PCIe
#define PCI_REG_STATUS_SIG_TARGET_ABORT             1 << 11
#define PCI_REG_STATUS_RECEIVED_TARGET_ABORT        1 << 12
#define PCI_REG_STATUS_RECIEVED_MASTER_ABORT        1 << 13
#define PCI_REG_STATUS_SIGNALED_SYSTEM_ERROR        1 << 15
#define PCI_REG_STATUS_DETECTED_PARITY_ERROR        1 << 16

#define PCI_BAR_MMIO                                (1 << 0)
#define PCI_BAR_64BIT                               (1 << 2)
#define PCI_BAR_PREFETCHABLE                        (1 << 3)
#define PCI_BAR_ADDR                                0x1fffffffff << 4

typedef struct {
    _bool PCIe;
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

    u16 subsystem_id;
    u16 subsystem_vendor_id;
    u8 revision_id;

    u8 selftest_status;
    u8 min_grant; //Note: doesnt apply to PCIe
    u8 max_latency; //Note: doesnt apply to PCIe
} pci_dev_info_t;

typedef struct{
    uptr addr;
    _bool mmio : 1;
    _bool prefetchable : 1;
} pci_bar_t;

// this will just overwrite the current aspace, since we only need one
void register_aspace(pci_aspace_t aspace);

// traverse the latest aspace.
_bool pci_init(void);

pci_multi_dev_t pci_find_devices_by_id(u16 vendor_id, u16 device_id);
pci_multi_dev_t pci_find_devices_by_class(u8 class_base, u8 class_sub);

pci_dev_info_t pci_get_device_info(pci_dev_addr_t addr);

u16 pci_read_command_register(pci_dev_addr_t addr);
void pci_write_command_register(pci_dev_addr_t addr, u16 val);

u16 pci_read_status_register(pci_dev_addr_t addr);
void pci_write_status_register(pci_dev_addr_t addr, u16 val);

pci_bar_t pci_get_bar(pci_dev_addr_t addr, u8 bar_index);

#define pci_get_bar1(addr) pci_get_bar(addr, 1)
#define pci_get_bar2(addr) pci_get_bar(addr, 2)
#define pci_get_bar3(addr) pci_get_bar(addr, 3)
#define pci_get_bar4(addr) pci_get_bar(addr, 4)
#define pci_get_bar5(addr) pci_get_bar(addr, 5)

u32 pci_read_custom_register(pci_dev_addr_t addr, u8 off);
void pci_write_custom_register(pci_dev_addr_t addr, u8 off, u32 val);

u8 get_capability_offset(pci_dev_addr_t addr, u8 cap_id);

#endif // __PCI_H__