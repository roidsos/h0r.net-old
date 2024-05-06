#include "kernel.h"
#include "wakeup.h"

struct HN_data_block data;
void kmain() {
    wakeup_init_hw();
    //wakeup_startup();

    while(true);
}
