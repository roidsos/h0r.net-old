#include "interrupts.h"
#include <arch/x86_64/interrupts/IDT.h>
#include <arch/x86_64/i8259.h>
#include <stdbool.h>

#define PIC_REMAP_OFFSET        0x20

void initialize_interrupts()
{
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    RegisterAllISRs();
}
void register_ISR(int irq,ISRHandler handler){
    ISR_RegisterHandler(irq,handler);
    EnableGate(irq);
    i8259_Unmask(irq);
}
void enable_interrupts(){
    enable_IDT();
    asm volatile("sti");
} 
void disable_interrupts(){
    asm volatile("cli");
} 