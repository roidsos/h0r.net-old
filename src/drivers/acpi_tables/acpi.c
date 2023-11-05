#include "acpi.h"
#include "madt.h"
#include <core/Memory/scubadeeznutz.h>
#include <core/kernel.h>
#include <core/logging/logger.h>
#include <types/string.h>

struct RSDP *rsdp = NULL;
struct RSDT *rsdt = NULL;
struct XSDT *xsdt = NULL;

bool use_xsdt = false;

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST, .revision = 0};

void acpi_init() {
    void *rsdp_addr = rsdp_request.response->address;
    // make sure we're not trying to access a nullptr
    if (rsdp_addr == NULL) {
        log_CRITICAL(NULL, HN_ERR_ACPI_FAULT, "RSDP Address is NULL!");
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
}

/**
 * This function traverses through the RSDT/XSDT and finds a table
 * with the specified signature.
 *
 * This function returns a virtual address of the found SDT;
 * 			or NULL if an SDT with the specified signature was not found.
 */
void *find_sdt(char *signature) {
    // amount of possible SDT pointers is (*SDT->Header.Length -
    // sizeof(*SDT->Header) / entry_divisor); entry_divisor is 8 if XSDT is
    // present; otherwise 4
    int entries =
        (use_xsdt ? ((xsdt->Header.Length - sizeof(xsdt->Header)) / 8)
                  : ((rsdt->Header.Length - sizeof(rsdt->Header)) / 4));

    // traverse through every possible entry
    for (int i = 0; i < entries; i++) {
        struct SDTHeader *Header = NULL;
        if (use_xsdt) {
            Header =
                (struct SDTHeader *)(uintptr_t)PHYS_TO_VIRT_HHDM(xsdt->SDTs[i]);
        } else {
            Header =
                (struct SDTHeader *)(uintptr_t)PHYS_TO_VIRT_HHDM(rsdt->SDTs[i]);
        }

        if (strncmp(Header->Signature, signature, 4)) {
            log_info("Found SDT with signature '%s'", signature);
            return (void *)Header;
        }
    }
    log_error("Could not fing ACPI table with signature \"%s\"", signature);
    return NULL;
}
