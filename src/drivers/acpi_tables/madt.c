#include "madt.h"
#include <drivers/Memory/scubadeeznutz.h>
#include <kernel.h>
#include <logging/logger.h>
#include <types/string.h>

void init_madt(struct MADT *madt) {
    // Make sure the MADT was found (!= NULL)
    if (madt == NULL) {
        return;
    }

    // What's left?
    // - Traverse the MADT
    // - Parse MADT entries
    // - Initialize APIC
}