#include "scubadeeznutz.h"
#include <drivers/Memory/Heap.h>
#include <drivers/Memory/PFA.h>
#include <arch/x86_64/interrupts/interrupts.h>

// "hippity hoppity,SCUBA DEEZ NUTZ"
// https://github.com/elydre/profanOS/

scuba_directory_t *kernel_directory;
scuba_directory_t *current_directory;
uint64_t g_map_to_addr;

scuba_directory_t *scuba_get_kernel_directory() {
    return kernel_directory;
}

void *i_allign_calloc(uint64_t size) {
    void *ptr = (void *) aligned_alloc(0x1000, size); // we need to allign to 4KB
    memset(ptr, 0, size);
    return ptr;
}


/**************************
 *                       *
 *      MMU CONTROL      *
 *                       *
**************************/

void scuba_switch(scuba_directory_t *dir) {
    current_directory = dir;
    // switch to the new page directory
    asm volatile("mov %0, %%cr3":: "r"(dir));
}

void scuba_flush_tlb() {
    // flush the TLB

    uint64_t cr3;
    asm volatile("mov %%cr3, %0": "=r"(cr3));
    asm volatile("mov %0, %%cr3":: "r"(cr3));
}

/**************************
 *                       *
 *      SCUBA INIT       *
 *                       *
**************************/
void scuba_fault_handler(Registers* regs);

int scuba_init() {
    // allocate a page directory
    kernel_directory = scuba_directory_create(0);

    uint64_t physical_end = get_total_RAM();
    g_map_to_addr = physical_end;

    // map the memory to itself
    for (uint64_t i = 0; i < physical_end; i += 0x1000) {
        scuba_map(kernel_directory, i, i);
    }


    struct limine_memmap_entry* kernel_region = get_memmap_entry_of_type(LIMINE_MEMMAP_KERNEL_AND_MODULES); 
    // map the kernel memory to itself
    for (uint64_t i = kernel_region->base; i < kernel_region->base + kernel_region->length; i += 0x1000) {
        scuba_map(kernel_directory, i, i);
    }
    
    struct limine_memmap_entry* HEAP_region = get_memmap_entry_of_type(LIMINE_MEMMAP_USABLE); 
    // map the kernel memory to itself
    for (uint64_t i = HEAP_region->base; i < HEAP_region->base + HEAP_region->length; i += 0x1000) {
        scuba_map(kernel_directory, i, i);
    }

    struct limine_memmap_entry* fb_region = get_memmap_entry_of_type(LIMINE_MEMMAP_FRAMEBUFFER); 
    // map the FB memory to itself
    for (uint64_t i = fb_region->base; i < fb_region->base + fb_region->length; i += 0x1000) {
        scuba_map(kernel_directory, i, i);
    }
    // switch to the new page directory
    scuba_switch(kernel_directory);

    register_ISR(0xE,scuba_fault_handler);

    return 0;
}

/**************************
 *                       *
 *     SCUBA PROCESS     *
 *                       *
**************************/

void scuba_process_switch(scuba_directory_t *dir) {
    if (current_directory == dir) return;
    if (dir->pid == 1) return;

    // switch to the new page directory
    scuba_switch(dir);

    // flush the TLB
    scuba_flush_tlb();
}

/**************************
 *                       *
 *    SCUBA DIRECTORY    *
 *                       *
**************************/

scuba_directory_t *scuba_directory_create(int target_pid) {
    // allocate a page directory
    scuba_directory_t *dir = i_allign_calloc(sizeof(scuba_directory_t));

    dir->to_free_index = 0;
    dir->pid = target_pid;

    // setup directory entries
    for (int i = 0; i < 1024; i++) {
        dir->entries[i].present = 0;
        dir->entries[i].rw = 1;
        dir->entries[i].user = 1;
        dir->entries[i].accessed = 0;
        dir->entries[i].unused = 0;
        dir->entries[i].frame = 0;
    }

    return dir;
}

void scuba_directory_init(scuba_directory_t *dir) {
    // map the memory to itself
    for (uint64_t i = 0; i < g_map_to_addr; i += 0x1000) {
        scuba_map_from_kernel(dir, i, i);
    }

    // video memory
    uint64_t from, to;

    //if (vesa_does_enable()) {
    //    // pixel buffer
    //    from = (uint64_t) vesa_get_framebuffer();
    //    to = from + vesa_get_pitch() * vesa_get_height() * 4 + 0x1000;
    //}

    for (uint64_t i = from; i < to; i += 0x1000) {
        scuba_map_from_kernel(dir, i, i);
    }
}

void scuba_directory_destroy(scuba_directory_t *dir) {
    // free all page tables
    for (uint64_t i = 0; i < dir->to_free_index; i++) {
        free(dir->to_free[i]);
    }

    // free the page directory
    free(dir);
}

/**************************
 *                       *
 *   SCUBA MAP / UNMAP   *
 *                       *
**************************/

int scuba_map_func(scuba_directory_t *dir, uint64_t virt, uint64_t phys, int from_kernel) {
    // get the page table index
    uint64_t table_index = virt / 0x1000 / 1024;

    // get the page table
    scuba_page_table_t *table = dir->tables[table_index];

    // if the page table doesn't exist, create it
    if (!table) {
        if (from_kernel) {
            // use the kernel page table
            table = kernel_directory->tables[table_index];
            if (!table) {
                log_error("Cannot use non-existant kernel page table");
                return 1;
            }
            if (!table->pages[(virt / 0x1000) % 1024].present) {
                log_error("Address not mapped in kernel page table");
                return 1;
            }
        } else {
            // check if we have space to free
            if (dir->to_free_index >= SCUBA_MAX_TO_FREE) {
                log_error("Too many page tables to free");
                return 1;
            }

            // create a new page table
            table = i_allign_calloc(sizeof(scuba_page_table_t));
            dir->to_free[dir->to_free_index++] = table;
        }
        dir->tables[table_index] = table;

        dir->entries[table_index].present = 1;
        dir->entries[table_index].rw = 1;
        dir->entries[table_index].user = 1;
        dir->entries[table_index].accessed = 0;
        dir->entries[table_index].unused = 0;
        dir->entries[table_index].frame = (uint64_t) table / 0x1000;
    }

    if (from_kernel) return 0;

    // get the page index
    uint64_t page_index = (virt / 0x1000) % 1024;

    // map the page
    table->pages[page_index].frame = phys / 0x1000;
    table->pages[page_index].present = 1;
    table->pages[page_index].rw = 1;
    table->pages[page_index].user = 1;

    return 0;
}

int scuba_create_virtual(scuba_directory_t *dir, uint64_t virt, int count) {
    if (dir->to_free_index + count >= SCUBA_MAX_TO_FREE) {
        log_error("Too many pages to free");
        return 1;
    }

    // alloc a page
    uint64_t phys = (uint64_t) i_allign_calloc(0x1000 * count);

    if (!phys) {
        log_error("Failed to alloc page");
        return 1;
    }

    // add the page to the list of pages to free
    dir->to_free[dir->to_free_index++] = (void *) phys;

    // map the page
    for (int i = 0; i < count; i++) {
        if (!scuba_map(dir, virt + i * 0x1000, phys + i * 0x1000)) continue;
        log_error("Failed to map page");
        return 1;
    }

    return 0;
}

int scuba_unmap(scuba_directory_t *dir, uint64_t virt) {
    // get the page table index
    uint64_t table_index = virt / 0x1000 / 1024;

    // get the page table
    scuba_page_table_t *table = dir->tables[table_index];

    // if the page table doesn't exist, return
    if (!table) {
        log_error("Page table doesn't exist");
        return 1;
    }

    // get the page index
    uint64_t page_index = (virt / 0x1000) % 1024;

    // unmap the page
    table->pages[page_index].present = 0;
    return 0;
}

/**************************
 *                       *
 *    SCUBA GET PHYS     *
 *     ( not used )      *
**************************/

uint64_t scuba_get_phys(scuba_directory_t *dir, uint64_t virt) {
    // get the page table index
    uint64_t table_index = virt / 0x1000 / 1024;

    // get the page table
    scuba_page_table_t *table = dir->tables[table_index];

    // if the page table doesn't exist, return
    if (!table) return 0;

    // get the page index
    uint64_t page_index = (virt / 0x1000) % 1024;

    // get the page
    scuba_page_t *page = &table->pages[page_index];

    // if the page doesn't exist, return
    if (!page->present) return 0;

    // return the physical address
    return page->frame * 0x1000;
}

/***************************
 *                        *
 *    SCUBA PAGE FAULT    *
 *                        *
***************************/
void scuba_fault_handler(Registers* regs) {
    // get the faulting address
    uint64_t faulting_address, new_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    //TODO: multitasking
    int pid = 0;

    // check if the faulting address is after RUN_BIN_VBASE
    //if (faulting_address >= RUN_BIN_VBASE) {
        new_address = faulting_address - (faulting_address % 0x1000);
        if (scuba_create_virtual(current_directory, new_address, SCUBA_VEXPD)) {
            log_error("Failed to create virtual pages");
        } else {
            log_info("Created virtual pages for %x\n", faulting_address);
            return;
        }
    //} else {
    //    log_error("Page fault during %s at %x, pid %d, code %x\n",
    //            (err_code & 0x2) ? "write" : "read",
    //            faulting_address,
    //            pid,
    //            err_code
    //    );
    //    log_error("Page fault, killing process");
    //}


    //TODO: multitasking
    // exit with the standard segfault code
    //if (force_exit_pid(pid, 139)) {
    //    log_fatal("Failed to exit process");
    //}
}