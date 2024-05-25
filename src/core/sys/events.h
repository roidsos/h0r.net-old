#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <libk/stdint.h>

typedef struct{
    u32 pid;
    void (*callback)(u32 pid,void* private);
} subscriber;

typedef struct{
    u32 event;
    subscriber* subscribers;
    u32 num_subscribers;
} event_t;

u32 event_create();
void event_destroy(u32 event_id);
void event_subscribe(u32 pid,u32 event,void (*callback)(u32 pid,void* private));
void event_unsubscribe(u32 pid,u32 event);

void event_fire(u32 event_id,void* private);

u32 event_exit();


#endif // __EVENTS_H__