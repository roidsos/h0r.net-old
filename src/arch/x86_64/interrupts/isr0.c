
#include "softTSS.h"
#include <arch/x86_64/i8259.h>
#include <utils/logging/logger.h>

softTSS *special_isr_0_handlr(softTSS *state) {

    i8259_SendEndOfInterrupt(0);
    return state;
}
