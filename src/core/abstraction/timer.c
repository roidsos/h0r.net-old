#include "timer.h"
#include <arch/x86/PIT.h>

// TODO: make the goddamn thing usable and multithreading aware
uint64_t uptime_secs;
uint16_t uptime_milis;
uint64_t countdown;

uint64_t timer_get_uptime_secs() { return uptime_secs; }
uint64_t timer_get_uptime_milis() { return uptime_milis; }

void timer_millisecond_tick() {
    uptime_milis++;
    if (uptime_milis >= 1000) {
        uptime_secs++;
        uptime_milis = 0;
    }

    if (countdown != 0) {
        countdown--;
    }
}

void init_hw_timers() {
    pit_init();

    uptime_milis = 0;
    uptime_secs = 0;
    countdown = 0;
}

void timer_sleep(uint64_t millis) {
    countdown = millis;

    while (countdown != 0) {
        __asm__ volatile("hlt");
    }
}
