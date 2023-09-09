#include "DriverManager.h"

#include <drivers/acpi_tables/acpi.h>
#include <drivers/hid/keyboard.h>
#include <drivers/io/pci.h>
#include <drivers/misc/time.h>

void init_drivers() {
    rtc_init();
    initkeyboard();
    init_PCI();
    // ACPI can wait
    // TODO: finish ACPI
    // init_acpi(data.rsdp);
}

void load_driver_for_device(__attribute__((unused)) struct Device dev) {}
