#include "cpu.h"
#include <vendor/printf.h>

// "good artists borrow, great artists steal"
// https://github.com/elydre/profanOS/
void cpuid(uint32_t eax, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    asm volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(eax));
}
void stacktrace() {
    uint64_t *rbp;
    __asm__ volatile("movq %%rbp, %0\r\n" : "=r"(rbp) :);

    while (rbp) {
        uint64_t rip = *(rbp + 1);
        rbp = (uint64_t *)*rbp;

        if (!rip) {
            break;
        }
        printf_("0x%016llx\n", rip);
    }
    printf_("\n");
}

void get_cpu_capabilities(CPUInfo *cpuInfo) {
    uint32_t eax, ebx, ecx, edx;

    cpuid(0, &eax, &ebx, &ecx, &edx);
    snprintf(cpuInfo->vendor, sizeof(cpuInfo->vendor), "%.4s%.4s%.4s",
             (char *)&ebx, (char *)&edx, (char *)&ecx);

    cpuid(1, &eax, &ebx, &ecx, &edx);
    cpuInfo->family = (eax >> 8) & 0xF;
    cpuInfo->model = (eax >> 4) & 0xF;
    cpuInfo->stepping = eax & 0xF;
    cpuInfo->ext_family = (eax >> 20) & 0xFF;
    cpuInfo->ext_model = (eax >> 16) & 0xF;
    cpuInfo->features[0] = edx;
    cpuInfo->features[1] = ecx;
}
int sys_init_fpu() {
    // get if fpu is present
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    if (!(edx & (1 << 24)))
        return 1;
    // enable fpu
    asm volatile("fninit");
    asm volatile("fwait");
    asm volatile("clts");
    asm volatile("mov %cr0, %rax");
    asm volatile("and $0x9FFFFFFF, %eax");
    asm volatile("mov %rax, %cr0");
    asm volatile("mov %cr4, %rax");
    asm volatile("or $0x600, %rax");
    asm volatile("mov %rax, %cr4");
    return 0;
}