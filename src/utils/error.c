#include "error.h"
#include <klibc/stdlib.h>
#include <vendor/printf.h>

char *errors[] = {"HN_ERR_KERNEL_EXITED", "HN_ERR_NO_ACPI"};

#define BGCOL "\x1b[48;2;17;0;34m"
#define FGCOL "\x1b[38;2;170;119;0m"

void trigger_psod(int error_code, char *details) {
    dprintf("Error UwU : %s", errors[error_code]);

    // background
    printf(BGCOL "\x1b[2J");

    // title,error type and details
    printf(FGCOL "FUCKING HALT!\n");
    printf("%s\n\n", errors[error_code]);
    if (details)
        printf("%s\n\n\n", details);

    hcf();
}
