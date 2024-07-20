#ifndef LAPIC_H
#define LAPIC_H

#include <core/libk/stdbool.h>

#define LAPIC_APICID      0x20
#define LAPIC_APICVER     0x30
#define LAPIC_TASKPRIOR   0x80
#define LAPIC_EOI    0x0B0
#define LAPIC_LDR     	 0x0D0
#define LAPIC_DFR     	 0x0E0
#define LAPIC_SPURIOUS    0x0F0
#define LAPIC_ESR     	 0x280
#define LAPIC_ICRL        0x300
#define LAPIC_ICRH        0x310
#define LAPIC_LVT_TMR     0x320
#define LAPIC_LVT_PERF    0x340
#define LAPIC_LVT_LINT0   0x350
#define LAPIC_LVT_LINT1   0x360
#define LAPIC_LVT_ERR     0x370
#define LAPIC_TMRINITCNT  0x380
#define LAPIC_TMRCURRCNT  0x390
#define LAPIC_TMRDIV      0x3E0
#define LAPIC_LAST        0x38F
#define LAPIC_DISABLE     0x10000
#define LAPIC_SW_ENABLE   0x100
#define LAPIC_CPUFOCUS    0x200
#define LAPIC_NMI     	 (4<<8)
#define TMR_PERIODIC     0x20000
#define TMR_BASEDIV      (1<<20)
#define LVT_MASKED       (1<<16)

#include "ACPI/MADT.h"

_bool lapic_init();
void lapic_eoi();

void lapic_timer_stop();
void lapic_timer_oneshot(u64 ms, u8 vec);


#endif
