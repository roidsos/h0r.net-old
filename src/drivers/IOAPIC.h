#ifndef IOLAPIC_H
#define IOLAPIC_H

#include "ACPI/MADT.h"

#define IOLAPIC_REGSEL 0x0
#define IOLAPIC_IOWIN  0x10

#define IOLAPIC_ID     0x0
#define IOLAPIC_VER    0x01
#define IOLAPIC_ARB    0x02
#define IOLAPIC_REDTBL 0x10

bool ioapic_init();

void ioapic_mask(uint8_t id);
void ioapic_unmask(uint8_t id);

#endif
