#include "core/sys/krnlexec.h"
#include <core/mm/heap.h>
#include <libk/stdbool.h>
#include <vendor/printf.h>
#include <core/sys/krnlexec.h>
void syscall_test() {
    //WARN: Arch specific code in core/ + TODO: smh move to arch/ or make architecture neutral
    __asm__ volatile ("mov $0, %rax");
    __asm__ volatile ("int $0x80");
    printf("If this prints, sys_exit has failed\n"); // wont get here
}

void gaia_main(void) {
    printf("TODO: startup tasks\n");
    execute("syscall_test", syscall_test, sizeof(syscall_test),false);
    while (true);
}
