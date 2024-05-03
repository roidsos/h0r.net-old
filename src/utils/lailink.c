#include <vendor/printf.h>
#include <core/mm/heap.h>
#include <utils/error.h>
#include <drivers/ACPI/RSDT.h>

// OS-specific functions.
void *laihost_malloc(size_t size){
    return malloc(size);
}
void *laihost_realloc(void * old, size_t newsize,UNUSED size_t oldsize){
    return realloc(old,newsize);
}
void laihost_free(void *tofree,UNUSED size_t size){
    free(tofree);
}

void laihost_log(int lvl, const char *msg){
    dprintf("[%i] %s",lvl,msg);
}

void laihost_panic(const char *msg){
    trigger_psod(HN_ERR_NO_FB,(char*)msg,NULL);
}

void *laihost_scan(char *sig, size_t index){
    return find_nth_thingy(sig,index);
}
