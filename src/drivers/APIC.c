#include "APIC.h"
#include "ACPI/HPET.h"
#include <arch/x86_64/cpu.h>
#include <utils/error.h>
#include <arch/x86_64/io/portio.h>
#include <core/memory.h>
#include <vendor/printf.h>

uint32_t ticks = 0;

uint32_t rreg(uint16_t offset){
    return *((uint32_t*)((uint64_t)data.lapic_base + offset));
}

void wreg(uint16_t offset,uint32_t val){
    *((uint32_t*)((uint64_t)data.lapic_base + offset)) = val;
}

void apic_timer_stop(){
    wreg(APIC_TMRINITCNT, 0);
    wreg(APIC_LVT_TMR, LVT_MASKED);
}

void apic_timer_oneshot(uint64_t ms, uint8_t vec){
    apic_timer_stop();
    wreg(APIC_TMRDIV, 0);
    wreg(APIC_LVT_TMR, vec);
    wreg(APIC_TMRINITCNT, ms * ticks);
}

void init_apic(){
    init_madt();

    // make sure the leagacy PIC is disabled
    outb8(0x21, 0xFF);
    iowait();
    outb8(0xA1, 0xFF);
    iowait();

    //enable the APIC
    wreg(APIC_SPURIOUS,0x1FF);
    wreg(APIC_TASKPRIOR,0);

    //check for timer
    if(!init_hpet()){
        trigger_psod(HN_ERR_NO_ACPI, "no HPET found LMAO",NULL);
    }

    //Calibrate the timer
    apic_timer_stop();
    wreg(APIC_TMRDIV, 0);
    wreg(APIC_LVT_TMR, 0);
    wreg(APIC_TMRINITCNT, 0xFFFFFFFF);

    hpet_usleep(100);

    ticks = 0xFFFFFFFF - rreg(APIC_TMRCURRCNT);
    apic_timer_stop();
}

void EOI(){
    wreg(APIC_EOI,0);
}
