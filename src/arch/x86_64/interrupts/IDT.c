#include "IDT.h"
#include <utils/binary.h>
#include <utils/logging/logger.h>
struct IDT64 {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t ist;
  uint8_t types_attr;
  uint16_t offset_mid;
  uint32_t offset_high;
  uint32_t zero;
} __attribute__((packed));

struct IDT64 g_IDT[256];

void IDT_Load();

void SetGate(int interrupt, void *base, uint16_t segmentDescriptor,
             uint8_t flags) {
  g_IDT[interrupt].zero = 0;
  g_IDT[interrupt].ist = 0;
  g_IDT[interrupt].offset_low =
      (uint16_t)(((uint64_t)base & 0x000000000000ffff));
  g_IDT[interrupt].offset_mid =
      (uint16_t)(((uint64_t)base & 0x00000000ffff0000) >> 16);
  g_IDT[interrupt].offset_high =
      (uint32_t)(((uint64_t)base & 0xffffffff00000000) >> 32);
  g_IDT[interrupt].selector = segmentDescriptor;
  g_IDT[interrupt].types_attr = flags;
}

void enable_IDT() { IDT_Load(); }
