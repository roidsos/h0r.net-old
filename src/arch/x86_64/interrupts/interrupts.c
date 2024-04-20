#include "interrupts.h"
#include <arch/x86_64/i8259.h>
#include <arch/x86_64/interrupts/IDT.h>
#include <arch/x86_64/io/portio.h>

void initialize_interrupts() {
    init_ISR();
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    enable_IDT();
}
void register_ISR(int irq, ISRHandler handler) {
    ISR_RegisterHandler(irq, handler);
    i8259_Unmask(irq);
}
void EOI(int irq) { i8259_SendEndOfInterrupt(irq); }
void enable_interrupts() {
    __asm__ volatile("sti");
    // i8259_Unmask(PIC_REMAP_OFFSET);
}
void disable_interrupts() {
    i8259_Disable();
    __asm__ volatile("cli");
}
