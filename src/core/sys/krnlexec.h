#ifndef __KERNLEXEC_H__
#define __KERNLEXEC_H__

#include <libk/stdbool.h>
#include <libk/stdint.h>

void execute(const char* name, void (*func)(),u64 fsize, _bool user);

#endif // __KERNLEXEC_H__