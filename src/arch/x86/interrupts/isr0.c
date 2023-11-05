
#include <arch/x86/i8259.h>
#include <arch/x86/interrupts/interrupts.h>
#include <arch/x86/io/portio.h>

void schedule(Registers *regs);
void pit_int();
extern bool is_sched_active;
extern void *krnlcr3;

volatile uint64_t spisr_start;
void special_isr_0_handlr(__attribute__((unused)) Registers *state) {
    if (is_sched_active)
        __asm__ volatile("mov %0, %%cr3\r\n" : : "a"(krnlcr3));
    pit_int();
    schedule(state);
}
volatile uint64_t spisr_end;

void spisr_init() { register_ISR(PIC_REMAP_OFFSET, special_isr_0_handlr); }