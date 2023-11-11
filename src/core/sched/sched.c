#include "sched.h"
#include <core/Memory/PFA.h>
#include <core/Memory/scubadeeznutz.h>
#include <core/kernel.h>
#include <core/logging/logger.h>
#include <types/queue.h>
#include <types/vector.h>

#define DS_SIGKILL 1

extern uint64_t xhndlr_start;
extern uint64_t xhndlr_end;
extern uint64_t spisr_start;
extern uint64_t spisr_end;

bool is_sched_active;
uint64_t current_PID;
void *krnlcr3;

queue(uint64_t, process_queue);
vector_static(process_t, processes);

static bool not_first_tick;
void schedule(Registers *regs) {
    if (!is_sched_active)
        return;
    sched_next_process(regs);
}

void sched_next_process(Registers *regs) {
    if (not_first_tick && processes.data[current_PID].deadlysignal == 0)
        memcpy(&processes.data[current_PID].status, regs, sizeof(Registers));
    not_first_tick = true;

    while (true) {
        if (process_queue.fullness == 0) {
            log_CRITICAL(NULL, HN_ERR_UNIMPLEMENTED, "Init system died lmao");
        }
        uint64_t next;
        queue_remove(process_queue, next);
        process_t nextproc = vector_at(&processes, next);
        if (nextproc.status.rip != 0 &&
            processes.data[current_PID].deadlysignal == 0) {
            current_PID = next;
            memcpy(regs, &nextproc.status, sizeof(Registers));
            __asm__ volatile("mov %0, %%cr3\r\n" : : "a"(nextproc.cr3));
            queue_add(process_queue, next);
            return;
        }
    }
}
static uint64_t sched_next_process_end;
void sched_init() {
    not_first_tick = false;
    __asm__ volatile("movq %%cr3, %0\r\n" : "=r"(krnlcr3) :);
    queue_init(process_queue, uint64_t, 500);
    vector_init(&processes);
}
void sched_enable() { is_sched_active = true; }

void kill_process(uint64_t ID) {
    processes.data[ID].status.rip = 0;
    processes.data[ID].deadlysignal = DS_SIGKILL;
}

uint64_t sched_add_process(process_t newproc){
    vector_push_back(&processes, newproc);
    queue_add(process_queue, processes.len - 1);

    return processes.len - 1;
};

uint64_t create_process(void (*process_main)(),
                        __attribute__((unused)) uint64_t prog_size,
                        uint8_t pl_and_flags, bool krnl_mode) {
    process_t newproc;
    newproc.pl_and_flags = pl_and_flags;
    // instruction and stack pointers
    newproc.status.rip = (uint64_t)process_main;
    newproc.status.rsp = (uint64_t)request_pages(4);

    // page table
    if (krnl_mode) {
        __asm__ volatile("movq %%cr3, %0\r\n" : "=r"(newproc.cr3) :);
    } else {
        void *newcr3 = create_pml4();
        scuba_map(newcr3, (uint64_t)&xhndlr_start, VIRT_TO_PHYS(&xhndlr_start),
                  NUM_BLOCKS((uint64_t)&xhndlr_end - (uint64_t)&xhndlr_start),
                  VIRT_FLAGS_DEFAULT);
        scuba_map(newcr3, (uint64_t)&spisr_start, VIRT_TO_PHYS(&spisr_start),
                  NUM_BLOCKS((uint64_t)&spisr_end - (uint64_t)&spisr_start),
                  VIRT_FLAGS_DEFAULT);
        scuba_map(newcr3, (uint64_t)&sched_next_process,
                  VIRT_TO_PHYS(&sched_next_process),
                  NUM_BLOCKS((uint64_t)&sched_next_process_end -
                             (uint64_t)&sched_next_process),
                  VIRT_FLAGS_DEFAULT);
        scuba_map(newcr3, newproc.status.rsp, VIRT_TO_PHYS(newproc.status.rsp),
                  4, VIRT_FLAGS_USERMODE);
        scuba_map(newcr3, (uint64_t)process_main, VIRT_TO_PHYS(process_main),
                  NUM_BLOCKS(prog_size), VIRT_FLAGS_USERMODE);
        newproc.cr3 = (void *)VIRT_TO_PHYS_HHDM(PHYS_TO_VIRT(newcr3));
    }
    // flags,permission and the deadly signal byte
    newproc.status.rflags = 0x202;
    newproc.deadlysignal = 0;
    if ((pl_and_flags & 0x3) == 0) {
        newproc.status.cs = 0x08;
        newproc.status.ss = 0x10;
    } else if ((pl_and_flags & 0x3) == 1) {
        newproc.status.cs = 0x18;
        newproc.status.ss = 0x20;
    } else if ((pl_and_flags & 0x3) == 2) {
        newproc.status.cs = 0x28;
        newproc.status.ss = 0x30;
    } else if ((pl_and_flags & 0x3) == 3) {
        newproc.status.cs = 0x38;
        newproc.status.ss = 0x40;
    }

    return sched_add_process(newproc);
}