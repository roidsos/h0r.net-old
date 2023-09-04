#ifndef __SOFTTSS_H__
#define __SOFTTSS_H__

#include <stdint.h>

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rsp;
    uint64_t rflags;
    uint64_t rip;
    uint64_t rbp;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t XMM0_0;
    uint64_t XMM0_1;
    uint64_t XMM1_0;
    uint64_t XMM1_1;
    uint64_t XMM2_0;
    uint64_t XMM2_1;
    uint64_t XMM3_0;
    uint64_t XMM3_1;
    uint64_t XMM4_0;
    uint64_t XMM4_1;
    uint64_t XMM5_0;
    uint64_t XMM5_1;
    uint64_t XMM6_0;
    uint64_t XMM6_1;
    uint64_t XMM7_0;
    uint64_t XMM7_1;
    uint64_t XMM8_0;
    uint64_t XMM8_1;
    uint64_t XMM9_0;
    uint64_t XMM9_1;
    uint64_t XMM10_0;
    uint64_t XMM10_1;
    uint64_t XMM11_0;
    uint64_t XMM11_1;
    uint64_t XMM12_0;
    uint64_t XMM12_1;
    uint64_t XMM13_0;
    uint64_t XMM13_1;
    uint64_t XMM14_0;
    uint64_t XMM14_1;
    uint64_t XMM15_0;
    uint64_t XMM15_1;
    uint64_t cs;
    uint64_t ds;

    // EXTERNAL
    uint64_t Privilege;
    uint64_t CodeStart;
    uint64_t StackStart;
} __attribute__((packed)) softTSS;

#endif // __SOFTTSS_H__