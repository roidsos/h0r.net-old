#include "PS2.h"

#include <arch/x86_64/interrupts/interrupts.h>
#include <arch/x86_64/io/portio.h>
#include <drivers/APIC.h>
#include <drivers/IOAPIC.h>
#include <vendor/printf.h>

void handler(UNUSED Registers *regs) {
    uint8_t scancode = inb8(PS2_DATA);
    printf("0x%x\n", scancode);
    EOI();
}

void init_ps2() {
    register_ISR(33, handler);
    unmask(1);

    if (inb8(PS2_COMMAND) & 0x1) // initialize the ps2 controller
        inb8(PS2_DATA);
    outb8(PS2_COMMAND, 0x60);
    outb8(PS2_DATA, 0b01100101);

    outb8(PS2_COMMAND, 0xAE);
    outb8(PS2_DATA, 0xf4);
}
