#ifndef __SCUBADEEZNUTZ_H__
#define __SCUBADEEZNUTZ_H__

// "hippity hoppity,SCUBA DEEZ NUTZ"
// https://github.com/schkwve/luxe/

#include <types/stdtypes.h>
#include <types/vector.h>
#include <limine.h>
#include <core/kernel.h>

#define BLOCK_SIZE 0x1000
#define MEM_VIRT_OFF 0xffffffff80000000

#define VIRT_FLAG_PRESENT (1 << 0)
#define VIRT_FLAG_RW (1 << 1)
#define VIRT_FLAG_USER (1 << 2)
#define VIRT_FLAG_WTHRU (1 << 3)
#define VIRT_FLAG_CACHE_DISABLE (1 << 4)
#define VIRT_FLAG_WCOMB (1 << 7)

#define VIRT_FLAGS_DEFAULT (VIRT_FLAG_PRESENT | VIRT_FLAG_RW)
#define VIRT_FLAGS_MMIO (VIRT_FLAGS_DEFAULT | VIRT_FLAG_CACHE_DISABLE)
#define VIRT_FLAGS_USERMODE (VIRT_FLAGS_DEFAULT | VIRT_FLAG_USER)

#define NUM_BLOCKS(num) (((num) + BLOCK_SIZE - 1) / BLOCK_SIZE)

#define VIRT_TO_PHYS(a) ((uint64_t)(a)- (uint64_t)MEM_VIRT_OFF)
#define PHYS_TO_VIRT(a) ((uint64_t)(a)+ (uint64_t)MEM_VIRT_OFF)
#define PHYS_TO_VIRT_HHDM(addr) ((uint64_t)(addr) + (uint64_t)data.hhdm_addr)
#define VIRT_TO_PHYS_HHDM(addr) ((uint64_t)(addr) + (uint64_t)data.hhdm_addr)

typedef struct {
	uint64_t virt_addr;
	uint64_t phys_addr;
	uint64_t flags;
	uint64_t np;
} mem_map_t;

uint64_t* create_pml4();

void scuba_map(uint64_t *pml4, uint64_t virt_addr, uint64_t phys_addr,
			  uint64_t np, uint64_t flags);
void scuba_unmap(uint64_t *pml4, uint64_t virt_addr, uint64_t np);

void _scuba_map(uint64_t *pml4, uint64_t virt_addr, uint64_t phys_addr,
			   uint64_t flags);
void _scuba_unmap(uint64_t *pml4, uint64_t virt_addr);
uint64_t scuba_get_phys_addr(uint64_t *pml4, uint64_t virt_addr);

#endif // __SCUBADEEZNUTZ_H__