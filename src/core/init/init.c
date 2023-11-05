#include <core/kernel.h>

#include <core/Memory/PFA.h>
#include <drivers/driver.h>

#include <backends/fb.h>

#include <core/abstraction/timer.h>
#include <core/interface/desh.h>
#include <core/logging/logger.h>
#include <core/sched/sched.h>
#include <vendor/printf.h>

// Forward decls for drivers not worth making .h-s for
void spisr_init();

void init_HW() {
    // Init Memory stuff
    PFA_init();
    log_info("HW_Init Target reached: Memory\n");

    // Init x86 PC specific drivers
    init_hw_timers();
    time_get(&data.time);
    init_drivers();
    spisr_init();
    log_info("HW_Init Target reached: Drivers\n");

    enable_interrupts();
}

void init_sys() {
    printf("Date: %02d/%02d/%d\n", data.time.month, data.time.day_of_month,
           data.time.year);
    printf("Time: %02d:%02d:%02d\n", data.time.hours, data.time.minutes,
           data.time.seconds);
    DeshInit();

    while (true) {
        DeshUpdate();
    }
}
void syscall_test() {
    __asm__ volatile("int $0x80");
    while (true) {
    }
}
int syscall_test_end;

void init_syscall();
void initsys_start() {
    sched_init();
    init_syscall();
    create_process(init_sys, 0, 0, true);
    // create_process(syscall_test,
    //                (uint64_t)&syscall_test_end - (uint64_t)syscall_test, 0,
    //                false);
    sched_enable();
}