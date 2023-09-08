#include "DriverManager.h"

#include <drivers/misc/time.h>
#include <drivers/hid/keyboard.h>
#include <drivers/io/pci.h>
#include <drivers/acpi_tables/acpi.h>

void init_drivers() {
    rtc_init();
    initkeyboard();
    init_PCI();
    // ACPI can wait 
    // TODO: finish ACPI
    //init_acpi(data.rsdp);
}

void load_driver_for_device(struct Device dev) {}
