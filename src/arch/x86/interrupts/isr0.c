
#include <arch/x86/i8259.h>
#include <arch/x86/interrupts/interrupts.h>
#include <arch/x86/io/portio.h>
#include <logging/logger.h>

void schedule(Registers *regs);
void pit_int();

void special_isr_0_handlr(__attribute__((unused)) Registers *state) {
    pit_int();
    schedule(state);
}

void spisr_init() { register_ISR(PIC_REMAP_OFFSET, special_isr_0_handlr); }