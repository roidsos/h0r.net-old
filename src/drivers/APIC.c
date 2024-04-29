#include "APIC.h"
#include "ACPI/HPET.h"
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/interrupts/interrupts.h>
#include <utils/error.h>
#include <arch/x86_64/io/portio.h>
#include <core/memory.h>
#include <vendor/printf.h>

void set_apic_base(uintptr_t apic) {
    uint32_t hi = 0;
    uint32_t lo = (apic & 0xfffff0000) | APIC_BASE_MSR_ENABLE;

    #ifdef __PHYSICAL_MEMORY_EXTENSION__
        edx = (apic >> 32) & 0x0f;
    #endif

    wrmsr(APIC_BASE_MSR, lo, hi);
}

uintptr_t get_apic_base() {
    uint32_t lo, hi;
    rdmsr(APIC_BASE_MSR, &lo, &hi);

    #ifdef __PHYSICAL_MEMORY_EXTENSION__
    return (lo & 0xfffff000) | ((hi & 0x0f) << 32);
    #else
    return (lo & 0xfffff000);
    #endif
}

uint32_t rreg(uint16_t offset){
    return *((uint32_t*)((uint64_t)data.lapic_base + offset));
}

void wreg(uint16_t offset,uint32_t val){
    *((uint32_t*)((uint64_t)data.lapic_base + offset)) = val;
}

void timer(UNUSED Registers* regs){
    dprintf("a");
    EOI();
}

void init_apic(){
    init_madt();
    set_apic_base(get_apic_base());

    // make sure the leagacy PIC is disabled
    outb8(0x21, 0xFF);
    iowait();
    outb8(0xA1, 0xFF);
    iowait();

    wreg(APIC_SPURIOUS,0x100);
    wreg(APIC_TASKPRIOR,0x00);


    //check for timer
    if(!init_hpet()){
        trigger_psod(HN_ERR_NO_ACPI, "no HPET found",NULL);
    }


    //Calibrate the timer
    wreg(APIC_TMRDIV, 0x3);
    wreg(APIC_TMRINITCNT, 0xFFFFFFFF);

    hpet_usleep(10000);

    wreg(APIC_LVT_TMR, LVT_MASKED);

    uint32_t ticksIn10ms = 0xFFFFFFFF - rreg(APIC_TMRCURRCNT);

    register_ISR(32, timer);

    wreg(APIC_TMRINITCNT, 0xFFFFFFFF);
    wreg(APIC_LVT_TMR, 32 | TMR_PERIODIC);
    wreg(APIC_TMRDIV, 0x3);
    wreg(APIC_TMRINITCNT, ticksIn10ms);
}

void EOI(){
    wreg(APIC_EOI,0);
}
