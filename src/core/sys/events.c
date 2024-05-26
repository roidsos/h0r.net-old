#include "events.h"
#include "core/sys/sched/process.h"
#include "libk/binary.h"
#include <core/sys/sched/sched.h>
#include <core/mm/heap.h>
#include <utils/log.h>

event_t events[64] = {0};
u32 num_events = 0;

u32 event_create(){
    log_trace("event_create()\n");
    if(num_events < 64){
        events[num_events].event = num_events;
        events[num_events].subscribers = NULL;
        events[num_events].num_subscribers = 0;
        num_events++;
        return num_events-1;
    }
    return 0xffffffff;
}
void event_destroy(u32 event_id){
    log_trace("event_destroy(%d)\n",event_id);
    events[event_id].event = 0xffffffff;
}
void event_subscribe(u32 pid,u32 event,void (*callback)(u32 event_id,void* private)){
    log_trace("event_subscribe(%d,%d,%p)\n",pid,event,callback);
    if(event < num_events){
        events[event].subscribers = realloc(events[event].subscribers,(events[event].num_subscribers+1)*sizeof(subscriber));
        events[event].subscribers[events[event].num_subscribers].pid = pid;
        events[event].subscribers[events[event].num_subscribers].callback = callback;
        events[event].num_subscribers++;
    }
}
void event_unsubscribe(u32 pid,u32 event){
    log_trace("event_unsubscribe(%d,%d)\n",pid,event);
    if (event < num_events && events[event].num_subscribers > 0) {
        for (u32 i = 0; i < events[event].num_subscribers; i++) {
            if (events[event].subscribers[i].pid == pid) {
                for (u32 j = i; j < events[event].num_subscribers - 1; j++) {
                    events[event].subscribers[j] = events[event].subscribers[j + 1];
                }
                events[event].num_subscribers--;
                break;
            }
        }
    }
}

extern process_t processes[MAX_PROCESSES];

void event_fire(u32 event_id,void* private){
    log_trace("event_fire(%d,%p)\n",event_id,private);
    _bool was_scheduled = false;
    if(event_id < num_events){
        for(u32 i = 0; i < events[event_id].num_subscribers; i++){
            log_trace("proc %p, callback %p\n",processes,events[event_id].subscribers[i].callback);
            processes[events[event_id].subscribers[i].pid].regs.rip = (u64)events[event_id].subscribers[i].callback;
            processes[events[event_id].subscribers[i].pid].regs.rdi = (u64)event_id;
            processes[events[event_id].subscribers[i].pid].regs.rsi = (u64)private;
            FLAG_SET(processes[events[event_id].subscribers[i].pid].state_flags, SCHED_FLAGS_CHANGED);
            if(events[event_id].subscribers[i].pid == sched_current_pid){
                was_scheduled = true;
            }
        }
    }
    if(was_scheduled){
        // TODO: smh stop the timer to avoid double advance
        __asm__ volatile("int $0x20");
    }
}
void event_exit(){
    log_trace("event_exit()\n");
    sched_restore_state(sched_current_pid);
}