#ifndef ERROR_H
#define ERROR_H

//WARN: Arch specific code in utils/ + TODO: smh move to arch/ or make architecture neutral
#include <arch/x86_64/interrupts/ISR.h>
// 0-31: Interrupt error thingies IDK
#define HN_ERR_KERNEL_EXITED  32
#define HN_ERR_NO_ACPI        33
#define HN_ERR_NO_FB          34
#define HN_ERR_OUT_OF_MEM     35
#define HN_ERR_LAI_PANIC      36
#define HN_ERR_NO_FS          37

void trigger_psod(int error_code,char* details,Registers* regs);

#endif
