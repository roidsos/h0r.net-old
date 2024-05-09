#include "LAPIC.h"
#include "ACPI/HPET.h"
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/io/portio.h>
#include <core/memory.h>
#include <utils/error.h>
#include <vendor/printf.h>

uint32_t ticks = 0;

uint32_t rreg(uint16_t offset) {
    return *((uint32_t *)((uint64_t)data.lapic_base + offset));
}

void wreg(uint16_t offset, uint32_t val) {
    *((uint32_t *)((uint64_t)data.lapic_base + offset)) = val;
}

void lapic_timer_stop() {
    wreg(LAPIC_TMRINITCNT, 0);
    wreg(LAPIC_LVT_TMR, LVT_MASKED);
}

void lapic_timer_oneshot(uint64_t ms, uint8_t vec) {
    lapic_timer_stop();
    wreg(LAPIC_TMRDIV, 0);
    wreg(LAPIC_LVT_TMR, vec);
    wreg(LAPIC_TMRINITCNT, ms * ticks);
}

void lapic_init() {
    madt_init();

    // make sure the leagacy PIC is disabled
    outb8(0x21, 0xFF);
    iowait();
    outb8(0xA1, 0xFF);
    iowait();

    // enable the LAPIC
    wreg(LAPIC_SPURIOUS, 0x1FF);
    wreg(LAPIC_TASKPRIOR, 0);

    // check for timer
    if (!hpet_init()) {
        trigger_psod(HN_ERR_NO_ACPI, "no HPET found LMAO", NULL);
    }

    // Calibrate the timer
    lapic_timer_stop();
    wreg(LAPIC_TMRDIV, 0);
    wreg(LAPIC_LVT_TMR, 0);
    wreg(LAPIC_TMRINITCNT, 0xFFFFFFFF);

    hpet_usleep(100);

    ticks = 0xFFFFFFFF - rreg(LAPIC_TMRCURRCNT);
    lapic_timer_stop();
}

void lapic_eoi() { wreg(LAPIC_EOI, 0); }
