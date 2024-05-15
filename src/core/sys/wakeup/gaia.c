#include <core/mm/heap.h>
#include <vendor/printf.h>
#include <types/stdtypes.h>

#include <core/sys/resman/SIV.h>
#include <core/sys/resman/registery.h>
#include <utils/log.h>

void gaia_main(void){
    printf("TODO: startup tasks\n");

    hive_header* hive = read_hive("sys/reg/system.reg");
    if(hive == NULL){
        printf("Failed to read hive.reg\n");
        while(true);
    }
    printf("system hive's name: %s\n",hive->name);
    log_trace("loaded system hive\n");
    key_header* key = read_key(hive,"test");
    if(key == NULL){
        printf("Failed to read key: test\n");
        free_hive(hive);
        while(true);
    }
    printf("key's name: %s\n",key->name);
    log_trace("loaded test key\n");
    entry_header* entry = read_entry(key,"test");
    if(entry == NULL){
        printf("Failed to read entry: test\n");
        free_hive(hive);
        while(true);
    }
    printf("stwing Uwu: %s\n",(char*)entry + sizeof(entry_header));
    
    
    free_hive(hive);

 
    while(true);
}