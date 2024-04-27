#include "RSDT.h"
#include <core/memory.h>
#include <klibc/string.h>
#include <limine.h>
#include <vendor/printf.h>

RSDT *rsdt;
XSDT *xsdt;
bool use_xsdt = false;

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST, .revision = 0};

bool locate_rsdt() {
    XSDP *rsdp = (XSDP *)rsdp_request.response->address;
    if (rsdp->revision >= 2) {
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
sdt_header *find_thingy(char *signature) {
    if (use_xsdt) {
        int entries = (xsdt->h.length - sizeof(xsdt->h)) / 8;

        for (int i = 0; i < entries; i++) {
            sdt_header *h =
                (sdt_header *)(uintptr_t)PHYS_TO_VIRT(xsdt->SDTs[i]);
            if (!strncmp(h->signature, signature, 4))
                return h;
        }

    } else {
        int entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;

        for (int i = 0; i < entries; i++) {
            sdt_header *h =
                (sdt_header *)(uintptr_t)PHYS_TO_VIRT(rsdt->SDTs[i]);
            if (!strncmp(h->signature, signature, 4))
                return h;
        }
    }

    return NULL;
}