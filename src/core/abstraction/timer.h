#ifndef __TIMER_H__
#define __TIMER_H__
#include <types/stdtypes.h>

void init_hw_timers();
void timer_millisecond_tick();

uint64_t timer_get_uptime_secs();
uint64_t timer_get_uptime_milis();
void timer_sleep(uint64_t millis);

#endif // __TIMER_H__