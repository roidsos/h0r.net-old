#include <kernel.h>

#include <drivers/DriverManager.h>
#include <drivers/Memory/Heap.h>
#include <drivers/Memory/Memory.h>
#include <drivers/Memory/PFA.h>
#include <drivers/Memory/scubadeeznutz.h>
#include <drivers/io/serial.h>

#include "arch/x86/gdt.h"
#include <arch/x86/interrupts/interrupts.h>
#include <backends/fb.h>

#include "flanterm.h"
#include "utils/error-handling/falut-handler.h"
#include <arch/x86/PIT.h>
#include <interface/desh.h>
#include <limine.h>
#include <logging/logger.h>
#include <parsing/ini.h>
#include <sched/sched.h>
#include <vendor/printf.h>

uint64_t kernel_stack[8192];

// Forward decls for drivers not worth making .h-s for
void spisr_init();

void init_HW() {
    // Initialize screen and logger
    serial_init();
    logger_set_output(LOGGER_OUTPUT_COM1);
    log_info("HW_Init Target reached: IO\n");

    // Gather Data
    get_cpu_capabilities(&data.cpu_info);
    sys_init_fpu();
    gdt_init((uint64_t *)kernel_stack);
    log_info("HW_Init Target reached: CPU\n");

    // initialize interrupts
    initialize_interrupts();
    init_falut_handler();
    log_info("HW_Init Target reached: Interrupts\n");

    // Init Memory stuff
    mem_init();
    PFA_init();
    InitHeap(0x20000);
    log_info("HW_Init Target reached: Memory\n");

    // Init x86 PC specific drivers
    pit_init();
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
void load_config(struct limine_file *cfg_file) {
    // log_info("config file found");
    __attribute__((unused)) struct parsed_ini config =
        parse_ini(cfg_file->address);
}

void tar_init();
void load_initramfs(struct limine_file *tar_file) {
    data.initramfs = parse_tar(tar_file->address, tar_file->size);
    tar_init();
}
void syscall_test() {
    __asm__ volatile("int $0x80");
    while (true) {
    }
}
int syscall_test_end;

void init_syscall();
void init_sched() {
    sched_init();
    init_syscall();
    create_process(init_sys, 0, 0, true);
    // create_process(syscall_test,
    //                (size_t)&syscall_test_end - (size_t)&syscall_test, 3,
    //                false);
    sched_enable();
}