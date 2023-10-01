#ifndef __EVENT_H__
#define __EVENT_H__
#include <types/stdtypes.h>
typedef void* (*event_handler)(void*args);

struct event{
    //TODO: security stuff like permissions and shit
    bool exists;
    uint8_t subcount;
    event_handler* subscribers;
};

int create_event(uint8_t event_num);
void call_event(uint8_t event_num,void* args);
void subscribe(uint8_t event_num,void* (*event_handler)(void* args));

#endif // __EVENT_H__