#include "PCI.h"
#include <core/mm/heap.h>
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

pci_multi_dev_t pci_find_devices_by_id(u16 vendor_id, u16 device_id)
{
    pci_multi_dev_t ret = {
        .addrs = NULL,
        .count = 0
    };
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs = pci_aspace.read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                u16 vendor_id2 = pci_aspace.read(bus, device, function, 0x00);
                u16 device_id2 = pci_aspace.read(bus, device, function, 0x02);
                if (vendor_id == vendor_id2 && device_id == device_id2) {
                    pci_dev_addr_t addr = {
                        .bus = bus,
                        .dev = device,
                        .func = function
                    };
                    ret.addrs = realloc(ret.addrs, (ret.count + 1) * sizeof(pci_dev_addr_t));
                    ret.addrs[ret.count] = addr;
                    ret.count++;
                }
            }
        }
    }
    return ret;
}

pci_multi_dev_t pci_find_devices_by_class(u8 class_base, u8 class_sub)
{
    pci_multi_dev_t ret = {
        .addrs = NULL,
        .count = 0
    };
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numfuncs = pci_aspace.read(bus, device, 0, 0x0E) & (1 << 7) ? 8 : 1;
            for (int function = 0; function < numfuncs; function++) {
                u8 class_base2 = pci_aspace.read(bus, device, function, 0x0B);
                u8 class_sub2 = pci_aspace.read(bus, device, function, 0x0A);
                if (class_base == class_base2 && class_sub == class_sub2) {
                    pci_dev_addr_t addr = {
                        .bus = bus,
                        .dev = device,
                        .func = function
                    };
                    ret.addrs = realloc(ret.addrs, (ret.count + 1) * sizeof(pci_dev_addr_t));
                    ret.addrs[ret.count] = addr;
                    ret.count++;
                }
            }
        }
    }
    return ret;
}

pci_dev_info_t pci_get_device_info(pci_dev_addr_t addr)
{
    u8 header_type = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x0E) & 0x7F;
    if (header_type != 0) return (pci_dev_info_t){0};
    pci_dev_info_t ret = {
        .vendor_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x00),
        .device_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x02),

        .class_base = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x0B),
        .class_sub = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x0A),
        .prog_if = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x09),

        .capabilities = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x34),
        .interrupt_line = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3C),
        .interrupt_pin = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3D),

        .subsystem_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x2C),
        .subsystem_vendor_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x2E),
        .revision_id = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x08),

        .selftest_status = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3E),
        .min_grant = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3A),
        .max_latency = pci_aspace.read(addr.bus, addr.dev, addr.func, 0x3B)
    };
    return ret;
}

u16 pci_read_command_register(pci_dev_addr_t addr)
{
    return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x04) & 0xFFFF;
}

void pci_write_command_register(pci_dev_addr_t addr, u16 val)
{
    pci_aspace.write(addr.bus, addr.dev, addr.func, 0x04, val);
}

u16 pci_read_status_register(pci_dev_addr_t addr)
{
    return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x06) & 0xFFFF;
}

void pci_write_status_register(pci_dev_addr_t addr, u16 val)
{
    pci_aspace.write(addr.bus, addr.dev, addr.func, 0x06, val);
}

u32 pci_get_bar(pci_dev_addr_t addr, u8 bar_index)
{
    switch (bar_index) {
        case 0: return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x10);
        case 1: return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x14);
        case 2: return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x18);
        case 3: return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x1C);
        case 4: return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x24);
        case 5: return pci_aspace.read(addr.bus, addr.dev, addr.func, 0x28);
        default: return 0;
    }
}
