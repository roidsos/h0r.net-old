#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__
#include <arch/x86/interrupts/ISR.h>
#define PIC_REMAP_OFFSET        0x20

void initialize_interrupts();
void register_ISR(int irq,ISRHandler handler);
void enable_interrupts();
void disable_interrupts();
void EOI(int irq);

#endif // __INTERRUPTS_H__