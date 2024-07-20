#include <core/libk/macros.h>
#include "gdt.h"
#include "cpu.h"

gdt_table gdt = {
  {
  0x0000000000000000, // 0x00

  0x00009a000000ffff, // 0x08 16 bit code
  0x000093000000ffff, // 0x10 16 bit data

  0x00cf9a000000ffff, // 0x18 32 bit code
  0x00cf93000000ffff, // 0x20 32 bit data

  0x00af9b000000ffff, // 0x28 64 bit code cs
  0x00af93000000ffff, // 0x30 64 bit data ss

  0x00aff3000000ffff, // 0x38 data ss
  0x00affb000000ffff, // 0x40 user mode code cs
  },
  {0}
};

gdt_pointer gdtr;
tss daTSS = {0};

extern void load_gdt(gdt_pointer *);
extern void load_tss();

int gdt_init() {

    gdt.tss_entry.length = sizeof(tss);
    gdt.tss_entry.base = (u16)((u64)&daTSS & 0xffff);
    gdt.tss_entry.base1 = (u8)(((u64)&daTSS >> 16) & 0xff);
    gdt.tss_entry.flags = 0x89;
    gdt.tss_entry.flags1 = 0;
    gdt.tss_entry.base2 = (u8)(((u64)&daTSS >> 24) & 0xff);
    gdt.tss_entry.base3 = (u32)((u64)&daTSS >> 32);
    gdt.tss_entry.resv = 0;

    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (u64)&gdt;

	asm volatile("lgdt (%%rax)" : : "a"(&gdtr) : "memory");
	asm volatile("ltr %%ax" : : "a"(0x48));
    //asm volatile(
    //    "swapgs;"
	//	"mov $0, %%ax;"
	//	"mov %%ax, %%gs;"
	//	"mov %%ax, %%fs;"
	//	"swapgs;"
	//	"pushq $0x28;"
	//	"pushq $.reload;"
    //    "mov $0x30, %%ax;"
	//	"mov %%ax, %%ds;"
	//	"mov %%ax, %%es;"
	//	"mov %%ax, %%ss;"
	//	"retfq;"
	//	".reload:"
	//	: : : "ax");

    return 0;
}
void set_important_stacks(u64* kstack,u64* sched_stack){
    daTSS.rsp0 = (u64)kstack;
    daTSS.ist1 = (u64)sched_stack;
}
