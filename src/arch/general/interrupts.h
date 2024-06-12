#ifndef __GINTERRUPTS_H__
#define __GINTERRUPTS_H__

#include <libk/stdint.h>

void set_important_stacks(u64* kstack,u64* sched_stack);

#endif // __GINTERRUPTS_H__