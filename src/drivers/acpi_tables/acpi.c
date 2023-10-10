#include "acpi.h"
#include "madt.h"
#include <drivers/Memory/scubadeeznutz.h>
#include <kernel.h>
#include <logging/logger.h>
#include <types/string.h>

<<<<<<< HEAD
struct RSDP *rsdp;
struct XSDT *xsdt;
uint16_t xsdt_entries;
bool use_xsdt;
=======
struct RSDP *rsdp = NULL;
struct RSDT *rsdt = NULL;
struct XSDT *xsdt = NULL;
>>>>>>> eef1de8576deb0b9b12030f8576fe08beb1034d3

bool use_xsdt = false;

<<<<<<< HEAD
struct SDTHeader *find_table(struct XSDT *xsdt, char *signature) {
    struct SDTHeader *newSDTHeader;
    for (int t = 0; t < xsdt_entries; t++) {
        if (use_xsdt) {
            newSDTHeader = (struct SDTHeader *)PHYS_TO_VIRT_HHDM(
                xsdt->PointerToOtherSDT[t]);
        } else {
            newSDTHeader = (struct SDTHeader *)PHYS_TO_VIRT_HHDM(
                (uint64_t)((uint32_t *)xsdt->PointerToOtherSDT)[t]);
        }

        if (sign_check(newSDTHeader, signature))
            return newSDTHeader;
    }
    return 0;
}
void list_tables(struct XSDT *xsdt) {
    for (int t = 0; t < xsdt_entries; t++) {
        struct SDTHeader *newSDTHeader =
            (struct SDTHeader *)PHYS_TO_VIRT_HHDM(xsdt->PointerToOtherSDT[t]);
        log_info("ACPI table #%u: %c%c%c%c", t, newSDTHeader->Signature[0],
                 newSDTHeader->Signature[1], newSDTHeader->Signature[2],
                 newSDTHeader->Signature[3]);
    }
}
bool do_checksum(struct XSDT *table) {
    // thanks to @static.link on discord, he is the best
    uint8_t xsdp_checksum = 0;
    uint8_t *xsdp_ptr = (uint8_t *)table;
=======
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
>>>>>>> eef1de8576deb0b9b12030f8576fe08beb1034d3

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
}

/**
 * This function traverses through the RSDT/XSDT and finds a table
 * with the specified signature.
 * 
 * This function returns a virtual address of the found SDT;
 * 			or NULL if an SDT with the specified signature was not found.
 */
void *find_sdt(char *signature)
{
	// amount of possible SDT pointers is (*SDT->Header.Length - sizeof(*SDT->Header) / entry_divisor);
	// entry_divisor is 8 if XSDT is present; otherwise 4
	int entries = (use_xsdt ? ((xsdt->Header.Length - sizeof(xsdt->Header)) / 8) : 
								((rsdt->Header.Length - sizeof(rsdt->Header)) / 4));
	
	// traverse through every possible entry
	for (int i = 0; i < entries; i++) {
		struct SDTHeader *Header = NULL;
		if (use_xsdt) {
			Header = (struct SDTHeader *)(uintptr_t)PHYS_TO_VIRT_HHDM(xsdt->SDTs[i]);
		} else {
			Header = (struct SDTHeader *)(uintptr_t)PHYS_TO_VIRT_HHDM(rsdt->SDTs[i]);
		}

		if (strncmp(Header->Signature, signature, 4)) {
			log_info("Found SDT with signature '%s'", signature);
			return (void *)Header;
		}
	}

	return NULL;
}
