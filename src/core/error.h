#ifndef ERROR_H
#define ERROR_H

//WARN: Arch specific code in utils/ + TODO: smh move to arch/ or make architecture neutral
#include <arch/x86_64/interrupts/ISR.h>

#define HN_ERR_HW_EXCEPTION   0
#define HN_ERR_KERNEL_EXITED  1
#define HN_ERR_NO_ACPI        2
#define HN_ERR_NO_FB          3
#define HN_ERR_OUT_OF_MEM     4
#define HN_ERR_LAI_PANIC      5
#define HN_ERR_NO_FS          6

void trigger_psod(int error_code,char* details,Registers* regs);

#endif
