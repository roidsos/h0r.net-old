#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__
#include <arch/x86_64/interrupts/ISR.h>

void initialize_interrupts();
void register_ISR(int irq,ISRHandler handler);
void enable_interrupts();
void disable_interrupts();


#endif // __INTERRUPTS_H__