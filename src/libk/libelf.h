#ifndef __LIBELF_H__
#define __LIBELF_H__

#include <libk/stdint.h>

#define ELF_MAG0 0x7F
#define ELF_MAG1 'E'
#define ELF_MAG2 'L'
#define ELF_MAG3 'F'

#define ELF_RELOCATABLE 1
#define ELF_EXECUTABLE 2
#define ELF_SHARED 3
#define ELF_CORE 4

#define ELF_SEGMENT_NULL 0
#define ELF_SEGMENT_LOAD 1
#define ELF_SEGMENT_DYNAMIC 2
#define ELF_SEGMENT_INTERP 3
#define ELF_SEGMENT_NOTE 4

#define ELF_FLAGS_EXECUTABLE 1
#define ELF_FLAGS_WRITABLE 2
#define ELF_FLAGS_READABLE 4

#define AT_NULL 0
#define AT_PHDR 3
#define AT_PHENT 4
#define AT_PHNUM 5
#define AT_ENTRY 9

typedef struct {
	u64 type;
	u64 val;
} auxv64_t;

typedef struct {
	auxv64_t phdr;
	auxv64_t phnum;
	auxv64_t phent;
	auxv64_t entry;
	auxv64_t null;
} auxv64list_t;

typedef struct {
	u32 magic;
	u8 bits;
	u8 endianness;
	u8 headversion;
	u8 abi;
	u64 padding;
	u16 type;
	u16 isa;
	u32 elfver;
	u64 entry;
	u64 phpos;
	u64 shpos;
	u32 flags;
	u16 headsize;
	u16 phsize;
	u16 phcount;
	u16 shsize;
	u16 shcount;
	u16 shnames;
} __attribute__((packed)) elfheader64_t;

#define PH_TYPE_LOAD 1
#define PH_TYPE_DYNAMIC 2
#define PH_TYPE_INTERPRETER 3
#define PH_TYPE_NOTE 4
#define PH_TYPE_SHLIB 5
#define PH_TYPE_PHDR 6

typedef struct{
	u32 type;
	u32 flags;
	u64 offset;
	u64 memaddr;
	u64 undefined;
	u64 fsize;
	u64 msize;
	u64 alignment;
} __attribute__((packed)) elfph64_t;

#endif // __LIBELF_H__