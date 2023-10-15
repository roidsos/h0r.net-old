#include <kernel.h>

#include <drivers/DriverManager.h>
#include <drivers/Memory/Heap.h>
#include <drivers/Memory/Memory.h>
#include <drivers/Memory/PFA.h>
#include <drivers/Memory/scubadeeznutz.h>

#include "arch/x86/drop-in/ezgdt.h"
#include <arch/x86/interrupts/interrupts.h>
#include <backends/fb.h>

#include "flanterm.h"
#include "utils/error-handling/falut-handler.h"
#include <VFS/vfs.h>
#include <arch/x86/PIT.h>
#include <font/font_renderer.h>
#include <interface/desh.h>
#include <limine.h>
#include <logging/logger.h>
#include <parsing/ini.h>
#include <sched/sched.h>
#include <vendor/printf.h>

// Forward decls for drivers not worth making .h-s for
void spisr_init();

void init_HW() {
    // Initialize screen and logger
    logger_set_output(LOGGER_OUTPUT_COM1);
    fr_init(data.framebuffer);
    data.ft_ctx = flanterm_fb_simple_init(
        data.framebuffer->address, data.framebuffer->width,
        data.framebuffer->height, data.framebuffer->pitch);

    log_info("Kernel Init Target reached: IO\n");

    // Gather Data
    get_cpu_capabilities(&data.cpu_info);
    sys_init_fpu();

    load_default_gdt();
    log_info("Kernel Init Target reached: CPU\n");

    // initialize interrupts
    initialize_interrupts();
    init_falut_handler();
    log_info("Kernel Init Target reached: Interrupts\n");

    // Init Memory stuff
    mem_init();
    PFA_init();
    InitHeap(0x20000);
    log_info("Kernel Init Target reached: Memory\n");

    // Init x86 PC specific drivers
    pit_init();
    time_get(&data.time);
    log_info("Kernel Init Target reached: x86 PC drivers\n");

    // Init the HW drivers
    init_drivers();
    spisr_init();
    log_info("Kernel Init Target reached: Misc Drivers\n");

    enable_interrupts();
    log_info("Kernel Initialized Successfully\n");
}

void init_sys() {
    vfs_mount(0,"/");
    log_info("Kernel Init Target reached: VFS\n");
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
void syscall_test(){
    __asm__ volatile("int $0x32");
    while(true){}
}
int syscall_test_end;

void init_syscall();
void init_sched() {
    sched_init();
    init_syscall();
    create_process(init_sys,0,0,true);
    //create_process(syscall_test,(size_t)&syscall_test_end - (size_t)&syscall_test,3,false);
    sched_enable();
}