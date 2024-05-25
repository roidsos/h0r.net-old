#ifndef GDT_H
#define GDT_H

#include <libk/stdint.h>
#include <libk/stddef.h>

typedef struct
{
    u16 size;
    u32 offset;
} __attribute__((packed)) gdt_pointer;

typedef struct {
    u32 reserved0;
    u64 rsp0;
    u64 rsp1;
    u64 rsp2;
    u64 reserved1;
    u64 ist1;
    u64 ist2;
    u64 ist3;
    u64 ist4;
    u64 ist5;
    u64 ist6;
    u64 ist7;
    u64 reserved2;
    u16 reserved3;
    u16 iomap_base;
} __attribute__((packed)) tss;

typedef struct {
  u16 length;
  u16 base;
  u8  base1;
  u8  flags;
  u8  flags1;
  u8  base2;
  u32 base3;
  u32 resv;
} __attribute__((packed)) tss_entry;

typedef struct
{
    u16 limit0;
    u16 base0;
    u8 base1;
    u8 access;
    u8 limit1_flags;
    u8 base2;
} __attribute__((packed)) gdt_entry;

typedef struct {
  gdt_entry gdt_entries[9];
  tss_entry tss_entry;
} __attribute__((packed)) gdt_table;

int gdt_init(u64* rsp0);
void set_kernel_stack(u64* stack);

#endif