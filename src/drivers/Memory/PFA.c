#include "PFA.h"
#include <utils/logging/logger.h>

size_t total_mem    = 0;
size_t free_mem     = 0;
size_t used_mem     = 0;
size_t reserved_mem = 0;
bool initialized = false;
struct Bitmap page_bitmap;

void lock_page(void* addr){
    size_t index = (size_t)addr / 4096;
    bitmap_set(page_bitmap,index,false);
    free_mem -= 4096;
    used_mem += 4096;

}

void free_page(void* addr){
    size_t index = (size_t)addr / 4096;
    bitmap_set(page_bitmap,index,false);
    free_mem += 4096;
    used_mem -= 4096;
}

void reserve_page(void* addr){
    size_t index = (size_t)addr / 4096;
    bitmap_set(page_bitmap,index,false);
    free_mem -= 4096;
    reserved_mem += 4096;
}

void unreserve_page(void* addr){
    size_t index = (size_t)addr / 4096;
    bitmap_set(page_bitmap,index,false);
    free_mem += 4096;
    reserved_mem -= 4096;
}

void lock_pages(void* addr,size_t num){
    for (size_t i = 0; i < num; i++)
    {
        lock_page((void*)((size_t)addr + i*4096));
    }
}

void free_pages(void* addr,size_t num){
    for (size_t i = 0; i < num; i++)
    {
        free_page((void*)((size_t)addr + i*4096));
    }
}

void reserve_pages(void* addr,size_t num){
for (size_t i = 0; i < num; i++)
    {
        reserve_page((void*)((size_t)addr + i*4096));
    }
}

void unreserve_pages(void* addr,size_t num){
    for (size_t i = 0; i < num; i++)
    {
        unreserve_page((void*)((size_t)addr + i*4096));
    }
}
size_t get_free_RAM(){
    return free_mem;
}
size_t get_used_RAM(){
    return used_mem;
}
size_t get_reserved_RAM(){
    return reserved_mem;
}
size_t get_total_RAM(){
    return total_mem;
}

void initPFA(struct limine_memmap_response* memmap)
{
    if (initialized)
        return;

    //Get Largest Free Segment
    void* largest_free_memseg = NULL;
    size_t largest_free_memseg_size = 0;
    for (size_t i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry* desc = memmap->entries[i];
        if (desc->type == LIMINE_MEMMAP_USABLE && desc->length > largest_free_memseg_size){
            largest_free_memseg      = (void*)desc->base;
            largest_free_memseg_size = desc->length;
        }
    }
    total_mem = CalculateTotalMemorySize(memmap);
    free_mem  = total_mem;
    size_t bitmap_size = (total_mem / 4096 / 8) + 1;

    if(bitmap_size > largest_free_memseg_size)
        log_CRITICAL(0,"Page bitmap does not fit in largest free segment");
    
    log_info("%i",largest_free_memseg);
    log_info("%i",largest_free_memseg_size);

    //Initialize bitmap
    page_bitmap.size    =  bitmap_size;
    page_bitmap.buffer  =  largest_free_memseg;

    for (size_t i = 0; i < bitmap_size && i < 167600; i++)
    {
        ((char*)largest_free_memseg)[i] = 0;
    }
    
    //Lock Memory of the bitmap
    lock_pages(page_bitmap.buffer,(page_bitmap.size / 4096) + 1);

    //Reserve Non-Usable Memory
    for (size_t i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry* desc = memmap->entries[i];
        if (desc->type != LIMINE_MEMMAP_USABLE){
            reserve_pages(desc->base,desc->length / 4096);
        }
    }

    //Initialization Done
    initialized = true; 
}
