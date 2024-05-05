#include "RSDT.h"
#include <core/memory.h>
#include <klibc/string.h>
#include <limine.h>
#include <utils/log.h>
RSDT *rsdt;
XSDT *xsdt;
bool use_xsdt = false;

int ACPI_revision = 0;

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST, .revision = 0};

bool locate_rsdt() {
    XSDP *rsdp = (XSDP *)rsdp_request.response->address;
    ACPI_revision = rsdp->revision;
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
sdt_header *find_nth_thingy(char *signature, size_t index) {
    log_trace("searching for the %uth \"%.4s\"!\n", index, signature);
    if (!strncmp("DSDT", signature, 4)) {
        fadt_header *fadt = (fadt_header *)find_nth_thingy("FACP", 0);
        if (use_xsdt) { // check for ACPI 2+
            return (sdt_header *)(uintptr_t)PHYS_TO_VIRT(fadt->X_Dsdt);
        } else {
            return (sdt_header *)(uintptr_t)PHYS_TO_VIRT(fadt->dsdt);
        }
    }
    size_t n = 0;
    if (use_xsdt) {
        int entries = (xsdt->h.length - sizeof(xsdt->h)) / 8;

        for (int i = 0; i < entries; i++) {
            sdt_header *h =
                (sdt_header *)(uintptr_t)PHYS_TO_VIRT(xsdt->SDTs[i]);
            if (!strncmp(h->signature, signature, 4)) {
                if (n == index)
                    return h;
                n++;
            }
        }

    } else {
        int entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;

        for (int i = 0; i < entries; i++) {
            sdt_header *h =
                (sdt_header *)(uintptr_t)PHYS_TO_VIRT(rsdt->SDTs[i]);
            if (!strncmp(h->signature, signature, 4)) {
                if (n == index)
                    return h;
                n++;
            }
        }
    }

    return NULL;
}
sdt_header *find_thingy(char *signature) {
    return find_nth_thingy(signature, 0);
}
