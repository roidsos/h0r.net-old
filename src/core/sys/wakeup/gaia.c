#include <core/mm/heap.h>
#include <libk/stdbool.h>
#include <libk/stddef.h>
#include <libk/stdint.h>
#include <vendor/printf.h>
#include <vendor/lai/include/lai/helpers/pm.h>
#include <utils/log.h>

void gaia_main(void) {
    printf("TODO: startup tasks\n");
    lai_enter_sleep(5);
    while (true)
        ;
}