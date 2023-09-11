#ifndef __ERROR_SCREEN_H__
#define __ERROR_SCREEN_H__
#include "error.h"
#include <arch/x86/interrupts/interrupts.h>

void trigger_error(int code,Registers* regs,char* details);

#endif // __ERROR_SCREEN_H__