#ifndef __SCUBADEEZNUTZ_H__
#define __SCUBADEEZNUTZ_H__

// "hippity hoppity,SCUBA DEEZ NUTZ"
// https://github.com/schkwve/luxe/

#include <types/stdtypes.h>
#include <types/vector.h>
#include <limine.h>

#define VIRT_FLAG_PRESENT (1 << 0)
#define VIRT_FLAG_RW (1 << 1)
#define VIRT_FLAG_USER (1 << 2)
#define VIRT_FLAG_WTHRU (1 << 3)
#define VIRT_FLAG_CACHE_DISABLE (1 << 4)
#define VIRT_FLAG_WCOMB (1 << 7)

#define VIRT_FLAGS_DEFAULT (VIRT_FLAG_PRESENT | VIRT_FLAG_RW)
#define VIRT_FLAGS_MMIO (VIRT_FLAGS_DEFAULT | VIRT_FLAG_CACHE_DISABLE)
#define VIRT_FLAGS_USERMODE (VIRT_FLAGS_DEFAULT | VIRT_FLAG_USER)

#define BLOCK_SIZE 0x1000
#define NUM_BLOCKS(num) (((num) + BLOCK_SIZE - 1) / BLOCK_SIZE)

extern struct limine_memmap_response *g_mmap;
extern struct limine_kernel_address_response *g_kernel_addr;

#define MEM_VIRT_OFF 0xffffffff80000000

#define VIRT_TO_PHYS(a) ((uint64_t)(a)- (uint64_t)MEM_VIRT_OFF)
#define PHYS_TO_VIRT(a) ((uint64_t)(a)+ (uint64_t)MEM_VIRT_OFF)

typedef struct {
	uint64_t virt_addr;
	uint64_t phys_addr;
	uint64_t flags;
	uint64_t np;
} mem_map_t;

typedef struct {
	uint64_t *pml4;
	vector_struct(uint64_t) mem_list;
} addr_space_t;

void scuba_init(struct limine_memmap_response *mmap,struct limine_kernel_address_response *kernel_addr);

void scuba_map(addr_space_t *ads, uint64_t virt_addr, uint64_t phys_addr,
			  uint64_t np, uint64_t flags, bool us);
void scuba_unmap(addr_space_t *ads, uint64_t virt_addr, uint64_t np, bool us);

addr_space_t *create_ads();

void _scuba_map(addr_space_t *ads, uint64_t virt_addr, uint64_t phys_addr,
			   uint64_t flags);
void _scuba_unmap(addr_space_t *ads, uint64_t virt_addr);
uint64_t scuba_get_phys_addr(addr_space_t *ads, uint64_t virt_addr);

#endif // __SCUBADEEZNUTZ_H__