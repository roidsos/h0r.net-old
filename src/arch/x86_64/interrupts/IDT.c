#include "IDT.h"
#include <libk/binary.h>
struct IDT64 {
    u16 offset_low;
    u16 selector;
    u8 ist;
    u8 types_attr;
    u16 offset_mid;
    u32 offset_high;
    u32 zero;
} PACKED;

struct IDT64 g_IDT[256];

void SetGate(int interrupt, void *base, u16 segmentDescriptor, u8 flags) {
    g_IDT[interrupt].zero = 0;
    g_IDT[interrupt].ist = 0;
    g_IDT[interrupt].offset_low = (u16)(((u64)base & 0x000000000000ffff));
    g_IDT[interrupt].offset_mid = (u16)(((u64)base & 0x00000000ffff0000) >> 16);
    g_IDT[interrupt].offset_high =
        (u32)(((u64)base & 0xffffffff00000000) >> 32);
    g_IDT[interrupt].selector = segmentDescriptor;
    g_IDT[interrupt].types_attr = flags;
}
