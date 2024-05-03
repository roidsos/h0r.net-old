#include "FADT.h"

//TODO: make it arch specific
#include <arch/x86_64/io/portio.h>

fadt_header* fadt;

bool init_fadt(){

    fadt_header* h =(fadt_header*)find_thingy("FACP");

    if (h == NULL){
        return false;
    }

    //al we have to do is make the header available...
    fadt = h;

    //..and get into ACPI mode
    if(fadt->SMI_command_port != 0
            && (fadt->ACPI_enable != 0 || fadt->ACPI_disable != 0))
        outb8(fadt->SMI_command_port, fadt->ACPI_enable);

    // *drum roll* wait for it
    while ((inb32(fadt->PM1a_control_block) & 1) == 0);
    return true;

}
