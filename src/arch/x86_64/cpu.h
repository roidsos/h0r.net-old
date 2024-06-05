#ifndef __CPU_H__
#define __CPU_H__

#define CPUID_GET_VENDORSTRING 0
#define CPUID_GET_FEATURES 1
#define CPUID_GET_TLB 2
#define CPUID_GET_SERIAL 3
#define CPUID_GET_INTEL_EXTENDED 0x80000000
#define CPUID_GET_INTEL_FEATURES 0x80000001
#define CPUID_GET_INTEL_BRANDSTRING1 0x80000002
#define CPUID_GET_INTEL_BRANDSTRING2 0x80000003
#define CPUID_GET_INTEL_BRANDSTRING3 0x80000004

#define CPUID_FEAT_ECX_SSE3        1 << 0
#define CPUID_FEAT_ECX_PCLMUL      1 << 1
#define CPUID_FEAT_ECX_DTES64      1 << 2
#define CPUID_FEAT_ECX_MONITOR     1 << 3
#define CPUID_FEAT_ECX_DS_CPL      1 << 4
#define CPUID_FEAT_ECX_VMX         1 << 5
#define CPUID_FEAT_ECX_SMX         1 << 6
#define CPUID_FEAT_ECX_EST         1 << 7
#define CPUID_FEAT_ECX_TM2         1 << 8
#define CPUID_FEAT_ECX_SSSE3       1 << 9
#define CPUID_FEAT_ECX_CID         1 << 10
#define CPUID_FEAT_ECX_SDBG        1 << 11
#define CPUID_FEAT_ECX_FMA         1 << 12
#define CPUID_FEAT_ECX_CX16        1 << 13
#define CPUID_FEAT_ECX_XTPR        1 << 14
#define CPUID_FEAT_ECX_PDCM        1 << 15
#define CPUID_FEAT_ECX_PCID        1 << 17
#define CPUID_FEAT_ECX_DCA         1 << 18
#define CPUID_FEAT_ECX_SSE4_1      1 << 19
#define CPUID_FEAT_ECX_SSE4_2      1 << 20
#define CPUID_FEAT_ECX_X2APIC      1 << 21
#define CPUID_FEAT_ECX_MOVBE       1 << 22
#define CPUID_FEAT_ECX_POPCNT      1 << 23
#define CPUID_FEAT_ECX_TSC         1 << 24
#define CPUID_FEAT_ECX_AES         1 << 25
#define CPUID_FEAT_ECX_XSAVE       1 << 26
#define CPUID_FEAT_ECX_OSXSAVE     1 << 27
#define CPUID_FEAT_ECX_AVX         1 << 28
#define CPUID_FEAT_ECX_F16C        1 << 29
#define CPUID_FEAT_ECX_RDRAND      1 << 30
#define CPUID_FEAT_ECX_HYPERVISOR  1 << 31
#define CPUID_FEAT_EDX_FPU         1 << 0
#define CPUID_FEAT_EDX_VME         1 << 1
#define CPUID_FEAT_EDX_DE          1 << 2
#define CPUID_FEAT_EDX_PSE         1 << 3
#define CPUID_FEAT_EDX_TSC         1 << 4
#define CPUID_FEAT_EDX_MSR         1 << 5
#define CPUID_FEAT_EDX_PAE         1 << 6
#define CPUID_FEAT_EDX_MCE         1 << 7
#define CPUID_FEAT_EDX_CX8         1 << 8
#define CPUID_FEAT_EDX_APIC        1 << 9
#define CPUID_FEAT_EDX_SEP         1 << 11
#define CPUID_FEAT_EDX_MTRR        1 << 12
#define CPUID_FEAT_EDX_PGE         1 << 13
#define CPUID_FEAT_EDX_MCA         1 << 14
#define CPUID_FEAT_EDX_CMOV        1 << 15
#define CPUID_FEAT_EDX_PAT         1 << 16
#define CPUID_FEAT_EDX_PSE36       1 << 17
#define CPUID_FEAT_EDX_PSN         1 << 18
#define CPUID_FEAT_EDX_CLFLUSH     1 << 19
#define CPUID_FEAT_EDX_DS          1 << 21
#define CPUID_FEAT_EDX_ACPI        1 << 22
#define CPUID_FEAT_EDX_MMX         1 << 23
#define CPUID_FEAT_EDX_FXSR        1 << 24
#define CPUID_FEAT_EDX_SSE         1 << 25
#define CPUID_FEAT_EDX_SSE2        1 << 26
#define CPUID_FEAT_EDX_SS          1 << 27
#define CPUID_FEAT_EDX_HTT         1 << 28
#define CPUID_FEAT_EDX_TM          1 << 29
#define CPUID_FEAT_EDX_IA64        1 << 30
#define CPUID_FEAT_EDX_PBE         1 << 31


#include <libk/stdint.h>
typedef struct {
    char vendor[13];       // Vendor ID (12 characters + null terminator)
    char brand_string[49]; // Brand String (48 characters + null terminator)
    u8 family;       // Family
    u8 model;        // Model
    u8 stepping;     // Stepping
    u8 ext_family;   // Extended Family
    u8 ext_model;    // Extended Model
    u32 features[4];  // CPU Capabilities (EDX, ECX)
} CPUInfo;
struct stackframe32_t {
    struct stackframe32_t* EBP;
    u32 EIP;
};
struct stackframe64_t {
    struct stackframe64_t* RBP;
    u64 RIP;
};
void cpuid(u32 eax, u32 *a, u32 *b, u32 *c, u32 *d);
void get_cpu_capabilities(CPUInfo *cpuInfo);
int sys_init_fpu();

u64 rdmsr(u32 msr);
void wrmsr(u32 msr, u64 val);
u64 read_tsc();

#endif // __CPU_H__
