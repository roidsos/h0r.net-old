
#include "user.h"
#include "cpu.h"

//thanks to https://github.com/asterd-og/ZanOS/blob/main/kernel/src/sys/user.c

void syscall_isr();

void user_init() {
  u64 efer = rdmsr(MSR_EFER);
  efer |= (1 << 0);
  wrmsr(MSR_EFER, efer);
  u64 star = 0;
  star |= ((u64)0x08 << 32); // kernel cs
  star |= ((u64)0x38 << 48); // user cs (it loads 0x30 + 16 for cs, which is 0x40 and + 8 for ss 0x38)
  wrmsr(MSR_STAR, star);
  wrmsr(MSR_LSTAR, (u64)syscall_isr);
  wrmsr(MSR_CSTAR, 0);
  wrmsr(MSR_CSTAR+1, 0x200);
}