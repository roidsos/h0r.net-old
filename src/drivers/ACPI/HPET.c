#include "HPET.h"
#include <core/mm/mem.h>
hpet_regs *regs;

// copied from: https://github.com/aurixos/os/blob/main/kernel/acpi/hpet.c

_bool hpet_init() {
    hpet_header *hpet = (hpet_header *)find_SDT("HPET");
    if (hpet == NULL) {
        return false;
    }
    regs = (hpet_regs *)PHYS_TO_VIRT(hpet->addr);
    regs->counter_val = 0;
    regs->general_config = 1;

    return true;
}

void hpet_usleep(u64 us) {
    u32 clock_period = regs->capabilities >> 32;
    volatile usize target_val =
        regs->counter_val + (us * (1000000000 / clock_period));
    while (regs->counter_val < target_val)
        ;
}
