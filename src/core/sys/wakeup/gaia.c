#include <libk/stdbool.h>
#include <vendor/printf.h>
#include <core/mm/heap.h>
#include "drivers/storage/AHCI.h"

void gaia_main(void) {
    printf("TODO: Startup tasks\n");
    char msgbuf[512];
    ahci_use_device(&abar->ports[0], 0,(u8*)msgbuf,1,false);
    printf("msgbuf: %s\n",msgbuf);
    while (true)
        ;
}
