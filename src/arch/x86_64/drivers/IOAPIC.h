#ifndef IOLAPIC_H
#define IOLAPIC_H

//Sorry, I forgot to attribute this
// this is based on: https://github.com/asterd-og/ZanOS/blob/main/kernel/src/dev/ioapic.c

#include "ACPI/MADT.h"

#define IOLAPIC_REGSEL 0x0
#define IOLAPIC_IOWIN  0x10

#define IOLAPIC_ID     0x0
#define IOLAPIC_VER    0x01
#define IOLAPIC_ARB    0x02
#define IOLAPIC_REDTBL 0x10

_bool ioapic_init();

void ioapic_mask(u8 id);
void ioapic_unmask(u8 id);

#endif
