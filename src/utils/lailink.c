#include <core/mm/heap.h>
#include <drivers/ACPI/RSDT.h>
#include <utils/error.h>
#include <vendor/printf.h>
#include <libk/macros.h>

// OS-specific functions.
void *laihost_malloc(usize size) { return malloc(size); }
void *laihost_realloc(void *old, usize newsize, usize oldsize) {
    return realloc_plus(old, newsize, oldsize);
}
void laihost_free(void *tofree, UNUSED usize size) { free(tofree); }

void laihost_log(int lvl, const char *msg) { dprintf("[%i] %s\n", lvl, msg); }

void laihost_panic(const char *msg) {
    trigger_psod(HN_ERR_LAI_PANIC, (char *)msg, NULL);
}

void *laihost_scan(char *sig, usize index) {
    return find_nth_thingy(sig, index);
}
