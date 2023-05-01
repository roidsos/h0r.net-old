#include <util/event.h>
#include <lib/printf.h>
#include <drivers/memory/Heap.h>

event* events = calloc(256*sizeof(event));

extern "C" void event_IRQ(uint_8 event_num,void* args){
    event E = events[event_num];
    for(int i = 0; i < E.subcount; i++){
        E.subscribers[i](args);
    }
}

int create_event(uint_8 event_num)
{
    if(!events[event_num].exists){
        events[event_num].exists = true;
        events[event_num].subscribers = calloc(256*sizeof(event_handler));
        events[event_num].subcount = 0;
    }
}

void call_event(uint_8 event_num,void* args)
{
    //call the event using interrupt 0x10, first argument is the event number, the second is the argument
    asm("int $0x10");
}

void subscribe(uint_8 event_num,event_handler e)
{
    if(events[event_num].exists){
        events[event_num].subscribers[events[event_num].subcount] = e;
        events[event_num].subcount++;
    }else{
        dprintf("Event does not exist");
    }
}
