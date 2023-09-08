
#include "softTSS.h"
#include <arch/x86_64/i8259.h>
#include <utils/logging/logger.h>
void pit_callback(softTSS *state);

softTSS *special_isr_0_handlr(softTSS *state) {

    log_info("special interrupt 0 recieved");
    pit_callback(state);

    i8259_SendEndOfInterrupt(0);
    return state;
}
