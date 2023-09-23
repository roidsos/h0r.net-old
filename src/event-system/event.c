#include "event.h"
#include <utils/heapdef.h>
#include <logging/logger.h>

struct event events[];

int create_event(uint8_t event_num)
{
    if(!events[event_num].exists){
        events[event_num].exists = true;
        events[event_num].subscribers = calloc(256*sizeof(event_handler));
        events[event_num].subcount = 0;
    }
}

void call_event(uint8_t event_num,void* args)
{
    struct event E = events[event_num];
    for(int i = 0; i < E.subcount; i++){
        E.subscribers[i](args);
    }
}

void subscribe(uint8_t event_num,event_handler e)
{
    if(events[event_num].exists){
        events[event_num].subscribers[events[event_num].subcount] = e;
        events[event_num].subcount++;
    }else{
        log_error("Event #%u does not exist",event_num);
    }
}