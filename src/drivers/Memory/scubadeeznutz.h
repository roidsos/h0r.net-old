#ifndef SCUBASUIT_H
#define SCUBASUIT_H

// "hippity hoppity,SCUBA DEEZ NUTZ"
// https://github.com/elydre/profanOS/

#include <stdint.h>
#include <limine.h>

// stadard page size
#define PAGE_SIZE 4096
#define SCUBA_MAX_TO_FREE 1024
#define SCUBA_VEXPD   16

// Max 4KB per page
typedef struct {
    uint32_t present :  1;
    uint32_t rw :       1;
    uint32_t user :     1;
    uint32_t accessed : 1;
    uint32_t dirty :    1;
    uint32_t unused :   7;
    uint32_t frame :   20;
} scuba_page_t;

// Max 4MB per table
typedef struct {
    scuba_page_t pages[1024];
} scuba_page_table_t;

typedef struct {
    uint32_t present :  1;
    uint32_t rw :       1;
    uint32_t user :     1;
    uint32_t accessed : 1;
    uint32_t dirty :    1;
    uint32_t unused :   7;
    uint32_t frame :   20;
} scuba_dir_entry_t;

// Max 4GB per directory
typedef struct {
    scuba_dir_entry_t entries[1024];
    scuba_page_table_t *tables[1024];

    uint32_t to_free_index;
    void *to_free[SCUBA_MAX_TO_FREE];

    uint32_t pid;
} scuba_directory_t;

#define scuba_map(dir, virt, phys) scuba_map_func(dir, virt, phys, 0)
#define scuba_map_from_kernel(dir, virt, phys) scuba_map_func(dir, virt, phys, 1)

scuba_directory_t *scuba_get_kernel_directory();

void scuba_init(struct limine_memmap_response* memmap);

scuba_directory_t *scuba_directory_create(int target_pid);
void scuba_directory_init(scuba_directory_t *dir);
void scuba_directory_destroy(scuba_directory_t *dir);

int scuba_map_func(scuba_directory_t *dir, uint64_t virt, uint64_t phys, int from_kernel);
int scuba_unmap(scuba_directory_t *dir, uint64_t virt);

int scuba_create_virtual(scuba_directory_t *dir, uint64_t virt, int count);

uint64_t scuba_get_phys(scuba_directory_t *dir, uint64_t virt);

#endif