#ifndef __TTY_H__
#define __TTY_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>

enum tty_type {
    TTY_TYPE_SERIAL = 1,
    TTY_TYPE_UTERUS = 2,
    TTY_TYPE_HOOKER = 3 //TODO: add hooker
};

enum tty_serial_port {
    TTY_SERIAL_PORT_COM1 = 0
};

typedef struct {
    u8 out_type;
    u8 out_port;
    u8 in_type;
    u8 in_port;
    u8 lock;
} tty_t;

u16 tty_register(tty_t tty);
void tty_unregister(u16 tty_id);

_bool tty_lock(u16 tty_id);
void tty_unlock(u16 tty_id);
u8 tty_get_lock(u16 tty_id);

void tty_write(u16 tty_id, char* data, u32 size);

extern _bool tty_initialized;

#endif