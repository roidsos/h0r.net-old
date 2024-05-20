#include "PS2.h"

#include <arch/x86_64/interrupts/interrupts.h>
#include <arch/x86_64/io/portio.h>
#include <arch/x86_64/drivers/IOAPIC.h>
#include <arch/x86_64/drivers/LAPIC.h>
#include <libk/macros.h>
#include <vendor/printf.h>

void handler(UNUSED Registers *regs) {
    u8 scancode = inb8(PS2_DATA);
    printf("0x%x\n", scancode);
    lapic_eoi();
}

void ps2_init() {
    register_ISR(33, handler);
    ioapic_unmask(1);

    if (inb8(PS2_COMMAND) & 0x1) // initialize the ps2 controller
        inb8(PS2_DATA);
    outb8(PS2_COMMAND, 0x60);
    outb8(PS2_DATA, 0b01100101);

    outb8(PS2_COMMAND, 0xAE);
    outb8(PS2_DATA, 0xf4);
}
