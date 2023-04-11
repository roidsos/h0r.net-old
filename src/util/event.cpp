#include <util/event.h>
#include <lib/printf.h>


extern "C" void event_IRQ(){
printf("E vent");
}

int create_event(uint_8 event_num)
{
    
}

void call_event(uint_8 event_num,void* args)
{
    
}

void subscribe(uint_8 event_num,void* (*event_handler)(void* args))
{
    
}
