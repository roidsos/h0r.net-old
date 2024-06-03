#ifndef __DEVMAN_H__
#define __DEVMAN_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>
#include <libk/stddef.h>

#define DEVICE_TYPE_BLOCK 0
#define DEVICE_TYPE_CHAR 1

typedef struct {
    u8 type;
    void* PP;
    union {
        struct {
            _bool (*read)(void* PP,u8* buf, u64 offset, u64 size);
            _bool (*write)(void* PP,u8* buf, u64 offset, u64 size);
        } blockdev;
        struct {
            h0r_char (*read)(void* PP);
            void (*write)(void* PP,h0r_char c); 
        } chardev;
    } u;
} device_t;
#endif // __DEVMAN_H__