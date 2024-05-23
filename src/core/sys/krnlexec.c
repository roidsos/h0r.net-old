#include <libk/stdint.h>
#include "arch/x86_64/interrupts/ISR.h"
#include "config.h"
#include "core/mm/mem.h"
#include "krnlexec.h"
#include <core/sys/sched/sched.h>
#include <arch/general/paging.h>
#include <core/mm/pmm.h>
#include <utils/log.h>

extern u64 text_start_ld;
extern u64 text_end_ld;
extern u64 rodata_start_ld;
extern u64 rodata_end_ld;
extern u64 data_start_ld;
extern u64 data_end_ld;

void execute(const char* name, void (*func)(),u64 fsize, _bool user){
    u64 pagemap = vmm_create_pagetable();
    Registers regs = {0};
    u64 stack = (u64)request_pages(2);

    log_trace("func: %p, stack: %p\n", func, stack);

    regs.rip = (u64)func;
    regs.rsp = stack;
    regs.cs = user ? 0x38 : 0x8;
    regs.ss = user ? 0x40 : 0x10;
    regs.rflags = 0x202;

    vmm_map_range(pagemap,stack,stack, 2,FLAGS_R | FLAGS_W | FLAGS_U);
    vmm_map_range(pagemap,(u64)func, (u64)VIRT_TO_PHYS(func),fsize / PAGE_SIZE,FLAGS_R | FLAGS_W | FLAGS_X | FLAGS_U);


    sched_add_process((char*)name, regs, pagemap);
}