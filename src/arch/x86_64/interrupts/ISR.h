#ifndef __ISR_H__
#define __ISR_H__
#include <libk/stdint.h>
#include <libk/macros.h>

typedef struct 
{
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;

	u64 interrupt;
	u64 error;

	u64 rip;
	u64 cs;
	u64 rflags;
	u64 rsp;
	u64 ss;
} PACKED Registers;

typedef void (*ISRHandler)(Registers* regs);

void init_ISR();
void ISR_RegisterHandler(int irq, ISRHandler handler);

#endif // __ISR_H__
