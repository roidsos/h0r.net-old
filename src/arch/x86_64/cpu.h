#ifndef __CPU_H__
#define __CPU_H__


// "good artists borrow, great artists steal"
// https://github.com/elydre/profanOS/
#include <stdint.h>
typedef struct {
    char vendor[13];  // Vendor ID (12 characters + null terminator)
    uint32_t family;  // Family
    uint32_t model;   // Model
    uint32_t stepping; // Stepping
    uint32_t ext_family; // Extended Family
    uint32_t ext_model;  // Extended Model
    uint32_t features[4]; // CPU Capabilities (EDX, ECX)
} CPUInfo;
struct stackframe {
    struct stackframe* ebp;
    uint32_t eip;
};
void cpuid(uint32_t eax, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);
void get_cpu_capabilities(CPUInfo *cpuInfo);
struct stackframe* get_stackframe();
void stacktrace();
int sys_init_fpu();

#endif // __CPU_H__