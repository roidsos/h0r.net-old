#include "PIT.h"
#include "interrupts/softTSS.h"
#include <drivers/io/portio.h>

void pit_set_divisor(uint16_t divisor) {
    if (divisor < 100)
        divisor = 100;
    outb8(0x40, (uint8_t)(divisor & 0x00ff));
    iowait();
    outb8(0x40, (uint8_t)((divisor & 0xff00) >> 8));
}
uint16_t pit_read_count() {
    uint16_t count = 0;

    asm("cli");

    // al = channel in bits 6 and 7, remaining bits clear
    outb8(0x43, 0b0000000);

    count = inb8(0x40);       // Low byte
    count |= inb8(0x40) << 8; // High byte

    asm("sti");

    return count;
}
void pit_set_count(uint16_t count) {
    // Disable interrupts
    asm("cli");

    outb8(0x40, count & 0xFF);          // Low byte
    outb8(0x40, (count & 0xFF00) >> 8); // High byte

    asm("sti");
    return;
}

void pit_callback(__attribute__((unused)) softTSS *state) {}

void pit_init() { pit_set_divisor(2000); }
