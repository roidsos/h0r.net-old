#include <drivers/io/portio.h>

// "good artists borrow, great artists steal"
// https://github.com/elydre/profanOS/
void sys_reboot() {
  char good = 0x02;
  while (good & 0x02)
    good = inb8(0x64);
  outb8(0x64, 0xFE);
  asm volatile("cli");
  asm volatile("hlt");
}
void sys_shutdown_vm_only() {
  outb16(0x604, 0x2000);  // qemu
  outb16(0xB004, 0x2000); // bochs
  outb16(0x4004, 0x3400); // virtualbox
  asm volatile("cli");
  asm volatile("hlt");
}