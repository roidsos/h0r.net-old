#ifndef __KERNLEXEC_H__
#define __KERNLEXEC_H__

#include <core/libk/stdbool.h>
#include <core/libk/stdint.h>

void execute(const char* name, void (*func)());

#endif // __KERNLEXEC_H__