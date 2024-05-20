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
} __attribute__((packed)) tss_entry_t;

typedef struct
{
    u16 limit0;
    u16 base0;
    u8 base1;
    u8 access;
    u8 limit1_flags;
    u8 base2;
} __attribute__((packed)) gdt_entry;

int gdt_init(u64* rsp0);
void set_kernel_stack(u64* stack);

#endif