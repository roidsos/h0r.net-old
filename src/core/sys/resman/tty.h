#ifndef __TTY_H__
#define __TTY_H__

#include <stdint.h>
#include <libk/stdtypes.h>

enum tty_type {
    TTY_TYPE_SERIAL = 1,
    TTY_TYPE_UTERUS = 2,
    TTY_TYPE_HOOKER = 3 //TODO: add hooker
};

enum tty_serial_port {
    TTY_SERIAL_PORT_COM1 = 0
};

typedef struct {
    uint8_t out_type;
    uint8_t out_port;
    uint8_t in_type;
    uint8_t in_port;
    uint8_t lock;
} tty_t;

uint16_t tty_register(tty_t tty);
void tty_unregister(uint16_t tty_id);

bool tty_lock(uint16_t tty_id);
void tty_unlock(uint16_t tty_id);
uint8_t tty_get_lock(uint16_t tty_id);

void tty_write(uint16_t tty_id, char* data, uint32_t size);

extern bool tty_initialized;

#endif