#include "kernel.h"
#include "sys/wakeup/wakeup.h"

struct HN_data_block data;
void kmain() {
    wakeup_init_hw();
    wakeup_do_mounts();
    wakeup_startup();

    while (true)
        ;
}
