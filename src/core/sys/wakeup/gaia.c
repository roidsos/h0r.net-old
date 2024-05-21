#include <core/mm/heap.h>
#include <libk/stdbool.h>
#include <vendor/printf.h>
#include <core/sys/sched/sched.h>
void syscall_test() {
    __asm__ volatile ("mov $0, %rax");
    __asm__ volatile ("int $0x80");
    printf("If this prints, sys_exit has failed\n"); // wont get here
}

void gaia_main(void) {
    printf("TODO: startup tasks\n");
    sched_add_process("syscall_test", syscall_test);
    while (true);
}
