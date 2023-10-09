#ifndef __MADT_H__
#define __MADT_H__

#include "acpi.h"

struct MADT {
	struct SDTHeader Header;
	uint32_t LAPICAddr;
	uint32_t Flags;
} __attribute__((packed));

struct APICHeader {
	uint8_t Type;
	uint8_t Length;
} __attribute__((packed));

struct APICLapic {
	struct APICHeader Header;
	uint8_t ACPIProcId;
	uint8_t APICId;
	uint32_t Flags;
} __attribute__((packed));

struct APICIOApic {
	struct APICHeader Header;
	uint8_t IOAPICid;
	uint8_t Reserved;
	uint32_t IOAPICAddr;
	uint32_t GSIBase;
} __attribute__((packed));

struct APICISO {
	struct APICHeader Header;
	uint8_t Bus;
	uint8_t IRQ;
	uint32_t GSI;
	uint16_t Flags;
} __attribute__((packed));

void init_madt(struct MADT *madt);

#endif // __MADT_H__