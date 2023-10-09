#include "acpi.h"
#include "madt.h"
#include <drivers/Memory/scubadeeznutz.h>
#include <kernel.h>
#include <logging/logger.h>
#include <types/string.h>

struct RSDP *rsdp = NULL;
struct RSDT *rsdt = NULL;
struct XSDT *xsdt = NULL;

bool use_xsdt = false;

// NOTE to @lolguy91:
// You might want to move this macro elsewhere
#define PHYS_TO_VIRT_HHDM(addr) ((uint64_t)(addr) + (uint64_t)data.hhdm_addr)

void init_acpi(void *rsdp_addr)
{
	// make sure we're not trying to access a nullptr
	if (rsdp_addr == NULL) {
		log_info("RSDP Address is NULL!");
		for(;;);
	}

	// we can safely assume that the rsdp is correct,
	// therefore we do not check for its checksum
	rsdp = (struct RSDP *)rsdp_addr;

	// ACPI > 2.0 = we should have an XSDT present
	if (rsdp->Revision >= 2) {
		use_xsdt = true;
	}

	// RSDT is mapped to the higher half
	rsdt = (struct RSDT *)(uintptr_t)PHYS_TO_VIRT_HHDM(rsdp->RSDTAddress);

	// and so is XSDT
	if (use_xsdt) {
		xsdt = (struct XSDT *)(uintptr_t)PHYS_TO_VIRT_HHDM(rsdp->XSDTAddress);
	}

	// finding other SDT's:
	// 
	// struct *table = (struct table *)find_sdt("SIGNATURE");
	// init_madt(table);
	//
	struct MADT *madt = (struct MADT *)find_sdt("APIC");
	init_madt(madt);

	// temporary infinite loop
	for(;;);
}

void *find_sdt(char *signature)
{
	(void)signature;
	log_warning("find_sdt() is unimplemented as of yet; returning NULL");
	return NULL;
}