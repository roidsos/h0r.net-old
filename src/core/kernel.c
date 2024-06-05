#include "kernel.h"
#include "sys/wakeup/wakeup.h"

u64 hn_pagemap; // separate cuz assembly
struct HN_data hn_data;
void kmain() {
    wakeup_init_hw();
    wakeup_do_mounts();
    wakeup_startup();

    while (true)
        ;
}
