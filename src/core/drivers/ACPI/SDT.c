#include "SDT.h"
#include <core/kernel.h>
#include <core/mm/mem.h>
#include <core/libk/string.h>
#include <core/utils/log.h>

RSDT *rsdt;
XSDT *xsdt;
_bool use_xsdt = false;

_bool do_checksum(sdt_header *table_header) {
    unsigned char sum = 0;

    for (u32 i = 0; i < table_header->length; i++) {
        sum += ((char *)table_header)[i];
    }
    if (sum != 0) {
        log_trace("Checksum failed on table \"%.4s\"!\n",
                  table_header->signature);
        return false;
    }
    return true;
}

_bool find_rsdt(void *rsdp_addr) {
    XSDP *rsdp = (XSDP *)rsdp_addr;
    hn_data.ACPI_ver = rsdp->revision;
    if (rsdp->revision >= 2) {
        log_trace("Using the XSDT!\n");
        use_xsdt = true;
        xsdt = (XSDT *)PHYS_TO_VIRT(rsdp->XSDT_address);
        if (!do_checksum(&xsdt->h)) {
            return false;
        }
    } else {
        rsdt = (RSDT *)PHYS_TO_VIRT(rsdp->RSDT_address);
        if (!do_checksum(&rsdt->h)) {
            return false;
        }
    }
    return true;
}

sdt_header *find_nth_SDT(char *signature, usize index) {
    log_trace("searching for the %uth \"%.4s\"!\n", index, signature);
    if (!strncmp("DSDT", signature, 4)) {
        fadt_header *fadt = (fadt_header *)find_nth_SDT("FACP", 0);
        if (use_xsdt) { // check for ACPI 2+
            return (sdt_header *)(uptr)PHYS_TO_VIRT(fadt->X_Dsdt);
        } else {
            return (sdt_header *)(uptr)PHYS_TO_VIRT(fadt->dsdt);
        }
    }
    usize n = 0;
    if (use_xsdt) {
        int entries = (xsdt->h.length - sizeof(xsdt->h)) / 8;

        for (int i = 0; i < entries; i++) {
            sdt_header *h = (sdt_header *)(uptr)PHYS_TO_VIRT(xsdt->SDTs[i]);
            if (!strncmp(h->signature, signature, 4)) {
                if (n == index)
                    return h;
                n++;
            }
        }

    } else {
        int entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;

        for (int i = 0; i < entries; i++) {
            sdt_header *h = (sdt_header *)(uptr)PHYS_TO_VIRT(rsdt->SDTs[i]);
            if (!strncmp(h->signature, signature, 4)) {
                if (n == index)
                    return h;
                n++;
            }
        }
    }

    return NULL;
}
sdt_header *find_SDT(char *signature) { return find_nth_SDT(signature, 0); }
