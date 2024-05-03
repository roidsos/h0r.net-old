#ifndef ERROR_H
#define ERROR_H

#include <arch/x86_64/interrupts/ISR.h>
// 0-31: Interrupt error thingies IDK
#define HN_ERR_KERNEL_EXITED  32
#define HN_ERR_NO_ACPI        33
#define HN_ERR_NO_FB          34
#define HN_ERR_OUT_OF_MEM     35

void trigger_psod(int error_code,char* details,Registers* regs);

#endif
