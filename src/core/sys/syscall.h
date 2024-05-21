#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <libk/stdint.h>

u64 syscall(u64 number, u64 arg1, u64 arg2, u64 arg3);

#endif // __SYSCALL_H__