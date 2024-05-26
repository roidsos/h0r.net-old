#include <core/mm/heap.h>
#include <core/sys/krnlexec.h>
#include <libk/stdbool.h>
#include <libk/macros.h>
#include <vendor/printf.h>

u64 sys_call(u64 number, u64 arg1, u64 arg2, u64 arg3) {
    u64 retval;
    __asm__ volatile("int $0x80" : "=a"(retval) : "a"(number), "D"(arg1), "S"(arg2), "d"(arg3) : "rcx", "r11");
    return retval;
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
