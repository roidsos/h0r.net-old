#include "cpu.h"
#include "libk/string.h"
void cpuid(u32 eax, u32 *a, u32 *b, u32 *c, u32 *d) {
    __asm__ volatile("cpuid"
                     : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                     : "a"(eax));
}

void get_cpu_capabilities(CPUInfo *cpuInfo) {
    u32 eax, ebx, ecx, edx;

    // Vendor String
    cpuid(CPUID_GET_VENDORSTRING, &eax, &ebx, &ecx, &edx);
    memcpy(&cpuInfo->vendor[0], &ebx, 4);
    memcpy(&cpuInfo->vendor[4], &edx, 4);
    memcpy(&cpuInfo->vendor[8], &ecx, 4);
    cpuInfo->vendor[12] = 0;

    cpuid(CPUID_GET_INTEL_BRANDSTRING1, &eax, &ebx, &ecx, &edx);
    memcpy(&cpuInfo->brand_string[0], &eax, 4);
    memcpy(&cpuInfo->brand_string[4], &ebx, 4);
    memcpy(&cpuInfo->brand_string[8], &ecx, 4);
    memcpy(&cpuInfo->brand_string[12], &edx, 4);
    cpuid(CPUID_GET_INTEL_BRANDSTRING2, &eax, &ebx, &ecx, &edx);
    memcpy(&cpuInfo->brand_string[16], &eax, 4);
    memcpy(&cpuInfo->brand_string[20], &ebx, 4);
    memcpy(&cpuInfo->brand_string[24], &ecx, 4);
    memcpy(&cpuInfo->brand_string[28], &edx, 4);
    cpuid(CPUID_GET_INTEL_BRANDSTRING3, &eax, &ebx, &ecx, &edx);
    memcpy(&cpuInfo->brand_string[32], &eax, 4);
    memcpy(&cpuInfo->brand_string[36], &ebx, 4);
    memcpy(&cpuInfo->brand_string[40], &ecx, 4);
    memcpy(&cpuInfo->brand_string[44], &edx, 4);
    cpuInfo->brand_string[48] = 0;

    cpuid(CPUID_GET_FEATURES, &eax, &ebx, &ecx, &edx);
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
    u32 eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    if (!(edx & (1 << 24)))
        return 1;
    // enable fpu
    __asm__ volatile("fninit");
    __asm__ volatile("fwait");
    __asm__ volatile("clts");
    __asm__ volatile("mov %cr0, %rax");
    __asm__ volatile("and $0x9FFFFFFF, %eax");
    __asm__ volatile("mov %rax, %cr0");
    __asm__ volatile("mov %cr4, %rax");
    __asm__ volatile("or $0x600, %rax");
    __asm__ volatile("mov %rax, %cr4");
    return 0;
}

void rdmsr(u32 msr, u32 *lo, u32 *hi) {
    __asm__ volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void wrmsr(u32 msr, u32 lo, u32 hi) {
    __asm__ volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

u64 read_tsc() {
    u64 result;
    __asm__ volatile("rdtsc" : "=A"(result));
    return result;
}
