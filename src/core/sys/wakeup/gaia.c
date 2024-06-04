#include <libk/stdbool.h>
#include <vendor/printf.h>
#include "core/sys/executive/ELF/elf.h"

void gaia_main(void) {
    printf("TODO: Startup tasks\n");
    exec_elf("bin/test", "test", false);
    while (true);
}
