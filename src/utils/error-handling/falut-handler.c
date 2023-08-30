#include <arch/x86_64/interrupts/interrupts.h>
#include <utils/logging/logger.h>
void GDT_falut_handler(Registers* regs){
    log_CRITICAL(HN_ERR_GDT_FALUT,"General Protection falut");
    EOI(0xD);
}
void init_falut_handler()
{
    register_ISR(PIC_REMAP_OFFSET + 0xD,GDT_falut_handler);
}
