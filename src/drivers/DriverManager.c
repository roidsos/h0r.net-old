#include "DriverManager.h"

#include <arch/x86/rtc.h>
#include <drivers/acpi_tables/acpi.h>
#include <drivers/hid/keyboard.h>
#include <drivers/io/pci.h>
#include <kernel.h>

void init_drivers() {
    rtc_init();
    initkeyboard();
    init_PCI();
    //init_acpi(data.rsdp);
}

void load_driver_for_device(__attribute__((unused)) struct Device dev) {}
