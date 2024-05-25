#include "kernel.h"
#include "sys/wakeup/wakeup.h"

struct HN_data hn_data;
void kmain() {
    wakeup_init_hw();
    wakeup_do_mounts();
    wakeup_startup();

    while (true)
        ;
}
