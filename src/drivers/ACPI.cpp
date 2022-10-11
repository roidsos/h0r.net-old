#include <io/io.h>
#include <util/stdint.h>

//TODO : https://forum.osdev.org/viewtopic.php?t=16990

void reboot() {// This is not ACPI its just a 8042 reset copied from osdev.org
  uint_8 good = 0x02;
  while (good & 0x02)
    good = inb8(0x64);
  outb8(0x64, 0xFE);
  asm("hlt");
}