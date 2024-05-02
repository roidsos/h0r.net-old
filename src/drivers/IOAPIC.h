#ifndef IOAPIC_H
#define IOAPIC_H

#include "ACPI/MADT.h"

#define IOAPIC_REGSEL 0x0
#define IOAPIC_IOWIN  0x10

#define IOAPIC_ID     0x0
#define IOAPIC_VER    0x01
#define IOAPIC_ARB    0x02
#define IOAPIC_REDTBL 0x10

bool init_ioapic();

void mask(uint32_t id);
void unmask(uint32_t id);

#endif
