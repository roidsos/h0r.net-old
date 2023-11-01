#ifndef __MADT_H__
#define __MADT_H__

#include "acpi.h"

struct MADT {
	struct SDTHeader header;
	uint32_t lapic_addr;
	uint32_t flags;
} __attribute__((packed));

struct APICHeader {
	uint8_t type;
	uint8_t length;
} __attribute__((packed));

struct LAPICEntry {
	struct APICHeader header;
	uint8_t ACPI_proc_id;
	uint8_t APIC_id;
	uint32_t flags;
} __attribute__((packed));

struct IOAPICEntry {
	struct APICHeader header;
	uint8_t IOAPIC_id;
	uint8_t reserved;
	uint32_t IOAPIC_addr;
	uint32_t GSI_base;
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