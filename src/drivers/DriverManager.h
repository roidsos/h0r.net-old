#ifndef __DRIVERMANAGER_H__
#define __DRIVERMANAGER_H__
#include <stdint.h>

#define HN_DEV_TYPE_PCI 0
#define HN_DEV_TYPE_PCIE 1
#define HN_DEV_TYPE_USB 2
struct Device
{
    uint8_t type;

    uint16_t vendor_id;
    uint16_t device_id;

    void* bus_native_struct;
};


void init_drivers();
void load_driver_for_device(struct Device dev);

#endif // __DRIVERMANAGER_H__