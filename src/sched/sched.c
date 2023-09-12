#include "sched.h"
#include <arch/x86/interrupts/interrupts.h>
#include <logging/logger.h>
#include <types/queue.h>
#include <types/vector.h>
typedef struct {
    Registers status;
    // TODO:add more stuff
} process_t;

static bool is_active;
static bool not_first_tick;
queue(uint64_t, process_queue);
vector_static(process_t, processes);
uint64_t current_process;

void schedule(Registers *regs) {
    if (!is_active)
        return;
    if (not_first_tick)
        memcpy(&processes.data[current_process].status, regs,
               sizeof(Registers));
    not_first_tick = true;

    while (true) {
        if (process_queue.fullness == 0) {
            log_CRITICAL(
                NULL, HN_ERR_UNIMPLEMENTED,
                "No more processes to execute, ideally, we should shut down");
        }
        uint64_t next;
        queue_remove(process_queue, next);
        process_t nextproc = vector_at(&processes, next);
        if (nextproc.status.rip != 0) {
            memcpy(regs, &nextproc.status, sizeof(Registers));
            current_process = next;
            queue_add(process_queue, next);
            break;
        }
    }
}
void sched_init() {
    queue_init(process_queue, uint64_t, 500);
    vector_init(&processes);
}
void sched_enable() { is_active = true; }

void kill_process(uint64_t ID) { processes.data[ID].status.rip = 0; }

uint64_t create_process(void (*process_main)()) {
    Registers newregs;
    __asm__ volatile("movq %%rsp, %0\r\n" : "=r"(newregs.rsp) :);
    newregs.rflags = 0x202;
    newregs.rip = (uint64_t)process_main;
    newregs.ss = 0x10;
    newregs.cs = 0x08;

    process_t newproc = {newregs};
    vector_push_back(&processes, newproc);
    queue_add(process_queue, processes.len - 1);

    return processes.len - 1;
}