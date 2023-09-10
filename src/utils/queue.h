#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>
#include <drivers/Memory/Heap.h>
#include <arch/x86_64/essential.h>

#define queue(type, name) struct { \
uint64_t size;       \
uint64_t unit_size;  \
type* content; \
uint64_t fullness; \
} name;   

#define queue_init(q,type,limit) q.unit_size = sizeof(type); \
q.size = sizeof(type) * limit; \
q.content = malloc(q.size); \
q.fullness = 0

#define queue_add(q,toadd) q.content[q.fullness] = toadd; \
q.fullness++

#define queue_remove(q,place2go) place2go = q.content[0]; \
memcpy(q.content,&q.content[1],(q.fullness--) * q.unit_size)

#endif // __QUEUE_H__