#ifndef __ISR_H__
#define __ISR_H__
#include <stdint.h>

typedef struct 
{
    // in the reverse order they are pushed:
    uint64_t ds;                                            // data segment pushed by us
    uint64_t edi, esi, ebp, useless, ebx, edx, ecx, eax;    // pusha
    uint64_t interrupt, error;                              // we push interrupt, error is pushed automatically (or our dummy)
    uint64_t eip, cs, eflags, esp, ss;                      // pushed automatically by CPU
} __attribute__((packed)) Registers;

typedef void (*ISRHandler)(Registers* regs);

void init_ISR();
void ISR_RegisterHandler(int irq, ISRHandler handler);

#endif // __ISR_H__