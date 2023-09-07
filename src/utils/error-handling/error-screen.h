#ifndef __ERROR-SCREEN_H__
#define __ERROR-SCREEN_H__
#include "error.h"
#include <arch/x86_64/interrupts/interrupts.h>

void trigger_error(int code,Registers* regs,char* details);

#endif // __ERROR-SCREEN_H__