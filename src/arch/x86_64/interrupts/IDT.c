#include "IDT.h"
#include <utils/binary.h>
typedef struct
{
    uint16_t BaseLow;
    uint16_t SegmentSelector;
    uint8_t Ist;
    uint8_t Flags;
    uint16_t BaseHigh;
    uint32_t BaseVeryHigh;
    uint32_t Zero;
} __attribute__((packed)) IDTEntry;

typedef struct
{
    uint16_t Limit;
    IDTEntry* Ptr;
} __attribute__((packed)) IDTDescriptor;


IDTEntry g_IDT[256];

IDTDescriptor g_IDTDescriptor = { sizeof(g_IDT) - 1, g_IDT };

void __attribute__((cdecl)) IDT_Load(IDTDescriptor* idtDescriptor);

void SetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags)
{
    g_IDT[interrupt].BaseLow = ((uint64_t)base) & 0xFFFF;
    g_IDT[interrupt].SegmentSelector = segmentDescriptor;
    g_IDT[interrupt].Ist = 0;// I have no TSS
    g_IDT[interrupt].Flags = flags;
    g_IDT[interrupt].BaseHigh = ((uint64_t)base >> 16) & 0xFFFF;
    g_IDT[interrupt].BaseVeryHigh = ((uint64_t)base >> 32) & 0xFFFFFFFF;
    g_IDT[interrupt].Zero = 0;
}

void EnableGate(int interrupt)
{
    FLAG_SET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void DisableGate(int interrupt)
{
    FLAG_UNSET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void enable_IDT()
{
   IDT_Load(&g_IDTDescriptor); 
}
