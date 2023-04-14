#ifndef __EVENT_H__
#define __EVENT_H__

#include <util/stdint.h>

typedef void* (*event_handler)(void*args);

struct event{
bool exists;
uint_8 subcount;
event_handler* subscribers;

};

int create_event(uint_8 event_num);
void call_event(uint_8 event_num,void* args);
void subscribe(uint_8 event_num,void* (*event_handler)(void* args));


#endif // __EVENT_H__