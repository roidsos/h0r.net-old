#include "elf.h"
#include "arch/general/paging.h"
#include "core/sys/sched/sched.h"
#include <arch/x86_64/interrupts/ISR.h>
#include <core/mm/pmm.h>
#include <core/sys/resman/SIV.h>
#include <libk/endian.h>
#include <libk/libelf.h>
#include <libk/macros.h>
#include <utils/log.h>
#include <vendor/printf.h>

void exec_elf(char *path, char *procname, _bool user) {
    elfheader64_t elf_header;
    u32 fd = siv_open(0, path, SIV_FLAGS_CACHE);
    siv_read(fd, 0, (char *)&elf_header, sizeof(elfheader64_t));

    if (be32toh(elf_header.magic) != ELF_MAGIC) {
        log_error("Invalid ELF at %s: Wrong magic value(0x%x)\n", path,
                  be32toh(elf_header.magic));
        siv_close(fd);
        return;
    }
    if (elf_header.type !=
        ELF_EXECUTABLE) { // TODO: support relocatable executables
        log_error("Invalid ELF at %s: Wrong type(0x%x)\n", path,
                  elf_header.type);
        siv_close(fd);
        return;
    }
    // start creating the process
    u64 pagemap = vmm_create_pagemap();
    Registers regs = {0};
    u64 stack = (u64)request_pages(2);

    for (u32 i = 0; i < elf_header.phcount; i++) {
        elfph64_t phdr;

        siv_read(fd, elf_header.phpos + sizeof(elfph64_t) * i, (char *)&phdr,
                 sizeof(elfph64_t));
        if (phdr.type == ELF_SEGMENT_LOAD) {
            void *seg = request_pages(DIV_ROUND_UP(phdr.msize, PAGE_SIZE));
            vmm_map_range(pagemap, phdr.memaddr, (u64)seg,
                          DIV_ROUND_UP(phdr.msize, PAGE_SIZE),
                          FLAGS_R | FLAGS_W | FLAGS_X | FLAGS_U);
            siv_read(fd, phdr.offset, (char *)seg, phdr.msize);
        }
    }
    regs.rip = (u64)elf_header.entry;
    regs.rsp = stack + STACK_SIZE * PAGE_SIZE;
    regs.cs = user ? 0x38 : 0x8;
    regs.ss = user ? 0x40 : 0x10;
    regs.rflags = 0x202;

    vmm_map_range(pagemap, stack, stack, STACK_SIZE,
                  FLAGS_R | FLAGS_W | FLAGS_U);
    sched_add_process((char *)procname, regs, pagemap);
}