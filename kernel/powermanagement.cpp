#include <io.h>
#include <stdint.h>

void reboot() {
  uint_8 good = 0x02;
  while (good & 0x02)
    good = inb8(0x64);
  outb8(0x64, 0xFE);
  asm("hlt");
}