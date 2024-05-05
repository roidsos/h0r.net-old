#include "HPET.h"
#include <core/memory.h>
hpet_regs *regs;

bool hpet_init() {
    hpet_header *hpet = (hpet_header *)find_thingy("HPET");
    if (hpet == NULL) {
        return false;
    }
    regs = (hpet_regs *)PHYS_TO_VIRT(hpet->addr);
    regs->counter_val = 0;
    regs->general_config = 1;

    return true;
}

void hpet_usleep(uint64_t us) {
    uint32_t clock_period = regs->capabilities >> 32;
    volatile size_t target_val =
        regs->counter_val + (us * (1000000000 / clock_period));
    while (regs->counter_val < target_val)
        ;
}
