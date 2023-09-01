#include "interrupts.h"
#include <drivers/io/portio.h>
#include <arch/x86_64/interrupts/IDT.h>
#include <arch/x86_64/i8259.h>
#include <stdbool.h>


void initialize_interrupts()
{
    RegisterAllISRs();
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    enable_IDT();
}
void register_ISR(int irq,ISRHandler handler){
    ISR_RegisterHandler(irq,handler);
    EnableGate(irq);
    i8259_Unmask(irq);
}
void EOI(int irq){
    i8259_SendEndOfInterrupt(irq);
}
void enable_interrupts(){
    asm volatile("sti");

    outb8(0x21,0xfd);
    outb8(0xa1,0xff);
} 
void disable_interrupts(){
    asm volatile("cli");
}
 