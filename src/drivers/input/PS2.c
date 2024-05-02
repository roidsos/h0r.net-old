#include "PS2.h"

#include <arch/x86_64/interrupts/interrupts.h>
#include <arch/x86_64/io/portio.h>
#include <vendor/printf.h>
#include <drivers/APIC.h>
#include <drivers/IOAPIC.h>

void handler(UNUSED Registers* regs){
    uint8_t scancode = inb8(0x64);
    printf("0x%x\n",scancode);
    EOI();
}

void init_ps2(){
    outb8(0x64,0xAE);
    outb8(0x60,0xf4);

    register_ISR(33, handler);
    unmask(1);
}
