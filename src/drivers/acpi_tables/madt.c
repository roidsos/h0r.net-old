#include "madt.h"
#include <core/Memory/scubadeeznutz.h>
#include <core/kernel.h>
#include <core/logging/logger.h>
#include <types/string.h>

static uint32_t lapic_addr;
uint32_t get_lapic_addr() { return lapic_addr; }

void init_madt(struct MADT *madt) {
    // Make sure the MADT was found (!= NULL)
    if (madt == NULL) {
        return;
    }
    lapic_addr = madt->lapic_addr;

    void *currentry = (void *)madt;
    while ((uint64_t)currentry - (uint64_t)madt > madt->header.Length) {
        uint8_t type = ((struct APICHeader *)currentry)->type;
        uint8_t length = ((struct APICHeader *)currentry)->length;
        switch (type) {
        case 0: // LAPIC for a core - _Ignored(since Limine feeds it to us)_
            break;

        case 1: // IOAPIC info - _Ignored_
            break;

        case 2: // IOAPIC source override - _Ignored_
            break;

        case 3: // IOAPIC NMI source - _Ignored_
            break;

        case 4: // LAPIC NMI - _Ignored_
            break;

        case 5: // Lapic override - _Ignored_
            break;

        case 9: // 64 bit LAPIC - _Ignored_
            break;

        default:
            log_CRITICAL(
                NULL, HN_ERR_ACPI_FAULT,
                "Corrupt MADT: Invalid type of APIC header at 0x%16llx",
                (uint64_t)currentry);
            break;
        }

        currentry += length;
    }

    // What's left?
    // - Initialize APIC - IDC about APIC, Ill need it for SMP tho
}