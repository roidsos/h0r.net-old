#include "tty.h"

#include <config.h>

#include <core/kernel.h>
#include <vendor/uterus/uterus.h>
#include <drivers/output/cereal.h>


tty_t ttys[MAX_TTYS];
uint16_t next_tty_id = 0;
bool tty_initialized;

uint16_t tty_register(tty_t tty) {
    if(next_tty_id == MAX_TTYS) return 0;
    if(next_tty_id == 0) tty_initialized = true; // registering tty0 initializes the tty system 
    ttys[next_tty_id] = tty;
    next_tty_id++;
    return next_tty_id - 1;
}

void tty_unregister(uint16_t tty_id) {
    ttys[tty_id] = (tty_t){0, 0, 0, 0, 0};
}

bool tty_lock(uint16_t tty_id)
{
    tty_t tty = ttys[tty_id];
    if(tty.lock == 0){
        tty.lock = 1;
        return true;
    }
    return false;
}

void tty_unlock(uint16_t tty_id)
{
    ttys[tty_id].lock = 0;
}

uint8_t tty_get_lock(uint16_t tty_id)
{
    return ttys[tty_id].lock;
}

void tty_write(uint16_t tty_id, char *buf, uint32_t size){
    tty_t tty = ttys[tty_id];
    if(tty.out_type == TTY_TYPE_SERIAL){
        for (uint32_t i = 0; i < size; i++) {
            cereal_write(buf[i], tty.out_port);
        }
    }else if (tty.out_type == TTY_TYPE_UTERUS){
        uterus_write(data.ut_ctx, buf, size);
    }
}