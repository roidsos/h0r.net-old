#include "tty.h"

#include <config.h>

#include <arch/x86_64/drivers/output/cereal.h>
#include <core/kernel.h>
#include <vendor/uterus/uterus.h>

tty_t ttys[MAX_TTYS];
u16 next_tty_id = 0;
_bool tty_initialized;

u16 tty_register(tty_t tty) {
    if (next_tty_id == MAX_TTYS)
        return 0;
    if (next_tty_id == 0)
        tty_initialized = true; // registering tty0 initializes the tty system
    ttys[next_tty_id] = tty;
    next_tty_id++;
    return next_tty_id - 1;
}

void tty_unregister(u16 tty_id) { ttys[tty_id] = (tty_t){0, 0, 0, 0, 0}; }

_bool tty_lock(u16 tty_id) {
    tty_t tty = ttys[tty_id];
    if (tty.lock == 0) {
        tty.lock = 1;
        return true;
    }
    return false;
}

void tty_unlock(u16 tty_id) { ttys[tty_id].lock = 0; }

u8 tty_get_lock(u16 tty_id) { return ttys[tty_id].lock; }

void tty_write(u16 tty_id, char *buf, u32 size) {
    tty_t tty = ttys[tty_id];
    if (tty.out_type == TTY_TYPE_SERIAL) {
        for (u32 i = 0; i < size; i++) {
            // WARN: Arch specific code in core/ + TODO: smh move to arch/ or
            // make architecture neutral
            cereal_write(buf[i], tty.out_port);
        }
    } else if (tty.out_type == TTY_TYPE_UTERUS) {
        uterus_write(data.ut_ctx, buf, size);
    }
}