#include "driver.h"

#include <arch/x86/rtc.h>
#include <drivers/acpi_tables/acpi.h>
#include <drivers/hid/ps2kb.h>
#include <drivers/io/pci.h>

void init_drivers() {
    rtc_init();
    kb_init();
    pci_init();
    acpi_init();
}

void load_driver_for_device(__attribute__((unused)) struct Device dev) {}
