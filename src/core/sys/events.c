#include "events.h"
#include "core/sys/sched/process.h"
#include <core/sys/sched/sched.h>
#include <core/mm/heap.h>

event_t events[64] = {0};
u32 num_events = 0;

u32 event_create(){
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
    events[event_id].event = 0xffffffff;
}
void event_subscribe(u32 pid,u32 event,void (*callback)(u32 pid,void* private)){
    if(event < num_events){
        events[event].subscribers = realloc(events[event].subscribers,(events[event].num_subscribers+1)*sizeof(subscriber));
        events[event].subscribers[events[event].num_subscribers].pid = pid;
        events[event].subscribers[events[event].num_subscribers].callback = callback;
        events[event].num_subscribers++;
    }
}
void event_unsubscribe(u32 pid,u32 event){
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

extern process_t* processes;

void event_fire(u32 event_id,void* private){
    _bool was_scheduled = false;
    if(event_id < num_events){
        for(u32 i = 0; i < events[event_id].num_subscribers; i++){
            sched_save_state(events[event_id].subscribers[i].pid);
            processes[events[event_id].subscribers[i].pid].regs.rip = (u64)events[event_id].subscribers[i].callback;
            processes[events[event_id].subscribers[i].pid].regs.rdi = (u64)event_id;
            processes[events[event_id].subscribers[i].pid].regs.rsi = (u64)private;
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
u32 event_exit(){
    sched_restore_state(sched_current_pid);
    __asm__ volatile("int $0x20");
    return 0;
}