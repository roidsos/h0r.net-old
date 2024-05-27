#include "krnlexec.h"
#include "arch/x86_64/interrupts/ISR.h"
#include "config.h"
#include <arch/general/paging.h>
#include <core/mm/pmm.h>
#include <core/sys/sched/sched.h>
#include <libk/stdint.h>
#include <utils/log.h>

void execute(const char *name, void (*func)()) {
    u64 pagemap = vmm_create_pagemap();
    Registers regs = {0};
    u64 stack = (u64)request_pages(STACK_SIZE);

    regs.rip = (u64)func;
    regs.rsp = stack + STACK_SIZE * PAGE_SIZE;
    regs.cs = 0x8;
    regs.ss = 0x10;
    regs.rflags = 0x202;

    vmm_map_range(pagemap, stack, stack, STACK_SIZE, FLAGS_R | FLAGS_W | FLAGS_U);
    sched_add_process((char *)name, regs, pagemap);
}