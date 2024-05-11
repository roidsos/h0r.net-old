#include <vendor/printf.h>
#include <types/stdtypes.h>

#include <core/sys/sched/scheduler.h>

void task1(){
    while(true){
        printf("a");
    }
}

void task2(){
    while(true){
        printf("b");
    }
}

void gaia_main(void){
    printf("TODO: startup tasks\n");
 
    //TODO: load Wakeup config and do startup tasks
    sched_add_process("proc1", task1);
    sched_add_process("proc2", task2);
 
    while(true);
}