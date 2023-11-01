#include "hwrand.h"
#include <arch/x86/cpu.h>
#include <core/logging/logger.h>

uint64_t rand(uint8_t source) {
    uint64_t result;
    uint32_t eax, ebx, ecx, edx;
    switch (source) {
    case HWRAND_SRC_RDRAND:
        cpuid(0x07, &eax, &ebx, &ecx, &edx);
        if ((ebx & (1 << 18)) == 0) {
            log_error("RDRAND not prensent, using RDSEED instead");
            goto rdseed_fallback;
        }
        asm volatile("rdrand %0" : "=r"(result));
        break;
    case HWRAND_SRC_RDSEED:
    rdseed_fallback:
        cpuid(0x01, &eax, &ebx, &ecx, &edx);
        if ((ebx & (1 << 30)) == 0) {
            log_CRITICAL(NULL, HN_ERR_UNIMPLEMENTED,
                         "RDSEED not prensent, nowhere to fall back to");
        }
        asm volatile("rdseed %0" : "=r"(result));
        break;

    default:
        cpuid(0x07, &eax, &ebx, &ecx, &edx);
        if ((ebx & (1 << 18)) != 0) {
            asm volatile("rdrand %0" : "=r"(result));
            break;
        }
        cpuid(0x01, &eax, &ebx, &ecx, &edx);
        if ((ebx & (1 << 30)) != 0) {
            asm volatile("rdseed %0" : "=r"(result));
            break;
        }
        log_CRITICAL(NULL, HN_ERR_UNIMPLEMENTED,
                     "RDSEED not prensent, nowhere to fall back to");
    }
    return result;
}
