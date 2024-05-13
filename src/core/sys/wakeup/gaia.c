#include <core/mm/heap.h>
#include <vendor/printf.h>
#include <types/stdtypes.h>

#include <core/sys/resman/SIV.h>
#include <core/sys/resman/registery.h>

void gaia_main(void){
    printf("TODO: startup tasks\n");

    hive_header* hive = read_hive("sys/reg/system.reg");
    if(hive == NULL){
        printf("Failed to read hive.reg\n");
        while(true);
    }
    printf("system hive's name: %s\n",hive->name);
    free_hive(hive);
 
    while(true);
}