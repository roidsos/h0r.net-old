#include "scheduler.h"
#include <arch/x86_64/interrupts/interrupts.h>
#include <vendor/printf.h>
#include <drivers/APIC.h>

void schedule(UNUSED Registers *regs){

    printf("a");

    apic_timer_oneshot(10, 32);
    EOI();
}

void init_sched(){
    register_ISR(32,schedule);
}
