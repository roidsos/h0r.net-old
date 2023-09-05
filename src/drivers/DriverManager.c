#include "DriverManager.h"

#include <drivers/hid/keyboard.h>
#include <drivers/io/pci.h>

void init_drivers() {
  initkeyboard();
  init_PCI();
}

void load_driver_for_device(struct Device dev) {}
