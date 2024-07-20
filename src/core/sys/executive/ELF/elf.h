#ifndef __ELF_H__
#define __ELF_H__

#include <core/libk/stdbool.h>
#include <core/libk/libelf.h>

void exec_elf(char* path,char* procname, _bool user);

#endif // __ELF_H__