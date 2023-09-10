#include "acpi.h"
#include <drivers/Memory/scubadeeznutz.h>
#include <utils/logging/logger.h>
#include <utils/string.h>

struct XSDT *xsdt;
uint16_t xsdt_entries;
struct RSDP *rsdp;

struct SDTHeader *find_table(struct XSDT *xsdt, char *signature) {
    for (int t = 0; t < xsdt_entries; t++) {
        struct SDTHeader *newSDTHeader =
            (struct SDTHeader *)PHYS_TO_VIRT(xsdt->PointerToOtherSDT[t]);
        for (int i = 0; i < 4; i++) {
            if (newSDTHeader->Signature[i] != signature[i]) {
                break;
            }
            if (t == 3)
                return newSDTHeader;
        }
    }
    return 0;
}
void list_tables(struct XSDT *xsdt) {
    for (int t = 0; t < xsdt_entries; t++) {
        struct SDTHeader *newSDTHeader =
            (struct SDTHeader *)PHYS_TO_VIRT(xsdt->PointerToOtherSDT[t]);
        log_info("%u", newSDTHeader->Length);
        // log_info("ACPI table #%u:
        // %c%c%c%c",t,newSDTHeader->Signature[0],newSDTHeader->Signature[1],newSDTHeader->Signature[2],newSDTHeader->Signature[3]);
    }
}
bool do_checksum(struct XSDT *table) {
    unsigned char sum = 0;

    for (uint32_t i = 0; i < table->h.Length; i++) {
        sum += ((char *)table)[i];
    }

    return sum == 0;
}

void init_acpi(void *rsdp_addr) {
    rsdp = (struct RSDP *)rsdp_addr;
    if (!(rsdp->Signature[0] == 'R' && rsdp->Signature[1] == 'S' &&
          rsdp->Signature[2] == 'D' && rsdp->Signature[3] == ' ' &&
          rsdp->Signature[4] == 'P' && rsdp->Signature[5] == 'T' &&
          rsdp->Signature[6] == 'R')) {
        log_error("RSDP not found");
        return;
    }
    log_info("RSDP found");

    xsdt = (struct XSDT *)PHYS_TO_VIRT(rsdp->XSDTAddress);
    xsdt_entries = (xsdt->h.Length - sizeof(xsdt)) / 8;

    log_info("XSDT Checksum: %u",xsdt->h.Checksum);
    log_info("XSDT CreatorID: %u",xsdt->h.CreatorID);
    log_info("XSDT CreatorRevision: %u",xsdt->h.CreatorRevision);
    log_info("XSDT Length: %u",xsdt->h.Length);
    log_info("XSDT Revision: %u",xsdt->h.Revision);
    log_info("XSDT Entries: %u",xsdt_entries);
    

    if(!do_checksum(xsdt)){
        log_error("XSDT corrupted or not found");
        return;
    }
    log_info("XSDT found");

    list_tables(xsdt);
    // struct SDTHeader* madt = find_table(xsdt,"APIC");
    // log_info("MADT found");
    // log_info("%u",madt->Length);

    log_info("ACPI initialized successfully");
}
