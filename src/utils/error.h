#ifndef ERROR_H
#define ERROR_H

#include <arch/x86_64/interrupts/ISR.h>
#define HN_ERR_KERNEL_EXITED    0
#define HN_ERR_NO_ACPI          1
// 2-34: Interrupt error thingies IDK

void trigger_psod(int error_code,char* details,Registers* regs);

#endif
