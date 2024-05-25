#include <core/mm/heap.h>
#include <core/sys/krnlexec.h>
#include <libk/stdbool.h>
#include <libk/macros.h>
#include <vendor/printf.h>

u64 sys_call(u64 number, u64 arg1, u64 arg2, u64 arg3) {
    u64 n,argc1,argc2,argc3;
    n = number;
    argc1 = arg1;
    argc2 = arg2;
    argc3 = arg3;
    __asm__ volatile("mov %%rax, %0\n\t" : "=r"(n));
    __asm__ volatile("mov %%rdi, %0\n\t" : "=r"(argc1));
    __asm__ volatile("mov %%rsi, %0\n\t" : "=r"(argc2));
    __asm__ volatile("mov %%rdx, %0\n\t" : "=r"(argc3));
    __asm__ volatile("int $0x20");
    u64 to_return = 0;
    __asm__ volatile("mov %%rax, %0\n\t" : "=r"(to_return));
    return to_return;
}

void test_handler(UNUSED u32 event_id,UNUSED void* private) {
    printf("Events Work!\n");
    //while (true);
    sys_call(6, 0, 0, 0); // sys_event_end
}

void gaia_main(void) {
    u64 event_id = sys_call(1, 0, 0, 0); // sys_event_create
    sys_call(3, event_id, (u64)test_handler, 0); // sys_event_subscribe

    sys_call(5, event_id, 0, 0); // sys_event_fire
    sys_call(2, event_id, 0, 0); // sys_event_destroy
    printf("TODO: Startup tasks\n");
    while (true)
        ;
}
