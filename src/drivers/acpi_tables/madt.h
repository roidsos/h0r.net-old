#ifndef __MADT_H__
#define __MADT_H__

#include "acpi.h"

struct MADT {
	struct SDTHeader Header;
	uint32_t LAPICAddr;
	uint32_t Flags;
} __attribute__((packed));

void init_madt(struct MADT *madt);

#endif // __MADT_H__