#include "PCI.h"
#include "libk/stdint.h"
#include <core/mm/heap.h>
#include <libk/stddef.h>
#include <utils/log.h>
pci_aspace_t pci_aspace = {.read = NULL, .write = NULL};

void iterate_pci() {
    log_trace("PCI Devices: \n");
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs =
                pci_aspace.read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                u16 vendor_id = pci_aspace.read(bus, device, function, 0x00);
                u16 device_id = pci_aspace.read(bus, device, function, 0x02);
                u8 class_base = pci_aspace.read(bus, device, function, 0x0B);
                u8 class_sub = pci_aspace.read(bus, device, function, 0x0A);
                u8 prog_if = pci_aspace.read(bus, device, function, 0x09);
                u8 header_type =
                    pci_aspace.read(bus, device, function, 0x0E) & 0x7F;
                if (device_id == 0x0000 || vendor_id == 0xFFFF) {
                    continue;
                }
                log_trace(
                    "  -VendorID: 0x%x,DeviceID: 0x%x,Class: 0x%x,Subclass: "
                    "0x%x,ProgIF: 0x%x,HeaderType: 0x%x \n",
                    vendor_id, device_id, class_base, class_sub, prog_if,
                    header_type);
            }
        }
    }
}

void register_aspace(pci_aspace_t aspace) { pci_aspace = aspace; }

_bool pci_init(void) {
    if (pci_aspace.read == NULL || pci_aspace.write == NULL)
        return false;

    iterate_pci();

    return true;
}

pci_multi_dev_t pci_find_devices_by_id(u16 vendor_id, u16 device_id) {
    pci_multi_dev_t ret = {.addrs = NULL, .count = 0};
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs =
                pci_aspace.read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                u16 vendor_id2 = pci_aspace.read(bus, device, function, 0x00);
                u16 device_id2 = pci_aspace.read(bus, device, function, 0x02);
                if (vendor_id == vendor_id2 && device_id == device_id2) {
                    pci_dev_addr_t addr = {
                        .bus = bus, .dev = device, .func = function};
                    ret.addrs = realloc(ret.addrs, (ret.count + 1) *
                                                       sizeof(pci_dev_addr_t));
                    ret.addrs[ret.count] = addr;
                    ret.count++;
                }
            }
        }
    }
    return ret;
}

pci_multi_dev_t pci_find_devices_by_class(u8 class_base, u8 class_sub) {
    pci_multi_dev_t ret = {.addrs = NULL, .count = 0};
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs =
                pci_aspace.read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                u8 class_base2 = pci_aspace.read(bus, device, function, 0x0B);
                u8 class_sub2 = pci_aspace.read(bus, device, function, 0x0A);
                if (class_base == class_base2 && class_sub == class_sub2) {
                    pci_dev_addr_t addr = {
                        .bus = bus, .dev = device, .func = function};
                    ret.addrs = realloc(ret.addrs, (ret.count + 1) *
                                                       sizeof(pci_dev_addr_t));
                    ret.addrs[ret.count] = addr;
                    ret.count++;
                }
            }
        }
    }
    return ret;
}

pci_dev_info_t pci_get_device_info(pci_dev_addr_t addr) {
    u8 header_type =
        pci_aspace.read(addr.bus, addr.dev, addr.func, 0x0E) & 0x7F;
    if (header_type != 0)
        return (pci_dev_info_t){0};
    pci_dev_info_t ret = {
        .addr = addr,
        .vendor_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x00),
        .device_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x02),

        .class_base = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x0B),
        .class_sub = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x0A),
        .prog_if = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x09),

        .capabilities = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x34),
        .interrupt_line = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3C),
        .interrupt_pin = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3D),

        .subsystem_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x2C),
        .subsystem_vendor_id =
            pci_aspace.read(addr.bus, addr.dev, addr.func, 0x2E),
        .revision_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x08),

        .selftest_status = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3E),
        .min_grant = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3A),
        .max_latency = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3B)};
    return ret;
}

u16 pci_read_command_register(pci_dev_addr_t addr) {
    return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x04) & 0xFFFF;
}

void pci_write_command_register(pci_dev_addr_t addr, u16 val) {
    pci_aspace.write(addr.bus, addr.dev, addr.func, 0x04, val);
}

u16 pci_read_status_register(pci_dev_addr_t addr) {
    return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x06) & 0xFFFF;
}

void pci_write_status_register(pci_dev_addr_t addr, u16 val) {
    pci_aspace.write(addr.bus, addr.dev, addr.func, 0x06, val);
}

pci_bar_t pci_get_bar(pci_dev_addr_t addr, u8 bar_index) {
    if (bar_index > 5)
        return (pci_bar_t){0};
    u32 barl =
        pci_aspace.read(addr.bus, addr.dev, addr.func, 0x10 + (4 * bar_index));
    u64 baraddr = barl & PCI_BAR_ADDR;
    if (barl & PCI_BAR_64BIT) {
        baraddr |= ((u64)pci_aspace.read(addr.bus, addr.dev, addr.func,
                                         0x14 + (4 * bar_index))
                    << 32);
    }
    baraddr &= ~((barl & PCI_BAR_MMIO) ? 0b1111 : 0b11);
    return (pci_bar_t){.addr = baraddr,
                       .mmio = (barl & PCI_BAR_MMIO),
                       .prefetchable = (barl & PCI_BAR_PREFETCHABLE)};
}

u32 pci_read_custom_register(pci_dev_addr_t addr, u8 off) {
    if (pci_aspace.read != NULL)
        return pci_aspace.read(addr.bus, addr.dev, addr.func, off);
    return 0;
}

void pci_write_custom_register(pci_dev_addr_t addr, u8 off, u32 val) {
    if (pci_aspace.write != NULL)
        pci_aspace.write(addr.bus, addr.dev, addr.func, off, val);
}

u8 get_capability_offset(pci_dev_addr_t addr, u8 cap_id) {
    if (!pci_aspace.PCIe) {
        if (!(pci_aspace.read(addr.bus, addr.dev, addr.func, 0x4) & (1 << 4)))
            return 0;
    }
    u32 cap_off = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x34);
    for (u8 try = 0; try < 32; try++) { // limit depth to 32
        u32 cap = pci_aspace.read(addr.bus, addr.dev, addr.func, cap_off);
        if ((cap & 0xFF) == cap_id) {
            return cap_off;
        }
        cap_off = (cap << 8) & 0xFF;
    }
    return 0;
}