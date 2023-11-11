#include <core/kernel.h>

#include <core/Memory/PFA.h>
#include <drivers/driver.h>

#include <backends/fb.h>

#include <core/abstraction/timer.h>
#include <core/interface/desh.h>
#include <core/logging/logger.h>
#include <core/sched/sched.h>
#include <core/VFS/vfs.h>
#include <types/string.h>
#include <vendor/printf.h>
#include <elf.h>

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
void run_elf(char* path){
    struct file_buffer torun = vfs_read(path,0,vfs_inspect(path)->ext.ext_file.size);

    Elf64_Ehdr* hdr = (Elf64_Ehdr*)torun.data;

    if(strncmp((char*)hdr->e_ident,ELFMAG,SELFMAG) != 0||
        hdr->e_ident[EI_CLASS] != ELFCLASS64 ||
        hdr->e_ident[EI_VERSION] != EV_CURRENT ||
        hdr->e_ident[EI_OSABI] != ELFOSABI_NONE ||
        hdr->e_machine != EM_X86_64 ||
        hdr->e_entry == 0){
        log_CRITICAL(NULL,HN_ERR_UNIMPLEMENTED,"bad elf");
    }

    Elf64_Phdr* phdr = (Elf64_Phdr*)(torun.data + hdr->e_phoff + sizeof(Elf64_Phdr));//TODO: this is a hack, make it proper
    void (*prog)() = (void (*)())(torun.data + phdr->p_offset + (hdr->e_entry - phdr->p_vaddr));
    prog();
}

void init_sys() {
    printf("Date: %02d/%02d/%d\n", data.time.month, data.time.day_of_month,
           data.time.year);
    printf("Time: %02d:%02d:%02d\n", data.time.hours, data.time.minutes,
           data.time.seconds);
    
    run_elf("/bin/test");

    log_CRITICAL(NULL,HN_ERR_UNIMPLEMENTED,"Bruh");
}

void init_syscall();
void initsys_start() {
    sched_init();
    init_syscall();
    create_process(init_sys, 0, 0, true);
    sched_enable();
}