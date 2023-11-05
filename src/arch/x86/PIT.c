#include "PIT.h"
#include <arch/x86/i8259.h>
#include <arch/x86/io/portio.h>
#include <core/abstraction/timer.h>

void pit_set_divisor(uint16_t divisor) {
    if (divisor < 100)
        divisor = 100;
    outb8(PIT_DATA, (uint8_t)(divisor & 0x00ff));
    iowait();
    outb8(PIT_DATA, (uint8_t)((divisor & 0xff00) >> 8));
}
uint16_t pit_read_count() {
    uint16_t count = 0;

    __asm__ volatile("cli");

    // al = channel in bits 6 and 7, remaining bits clear
    outb8(PIT_COMMAND, 0b0000000);

    count = inb8(PIT_DATA);       // Low byte
    count |= inb8(PIT_DATA) << 8; // High byte

    __asm__ volatile("sti");

    return count;
}
void pit_set_count(uint16_t count) {
    // Disable interrupts
    __asm__ volatile("cli");

    outb8(PIT_DATA, count & 0xFF);          // Low byte
    outb8(PIT_DATA, (count & 0xFF00) >> 8); // High byte

    __asm__ volatile("sti");
    return;
}

void pit_init() {
    pit_set_count(0);
    pit_set_divisor(1193182 / 1000);
}
void pit_int() { timer_millisecond_tick(); }