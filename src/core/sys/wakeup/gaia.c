#include <core/mm/heap.h>
#include <vendor/printf.h>
#include <types/stdtypes.h>

#include <core/sys/resman/SIV.h>
#include <core/sys/resman/registery.h>

void gaia_main(void){
    printf("TODO: startup tasks\n");

    uint32_t fd = siv_open(0,"hello.txt",SIV_INTENTS_READ);
    if(fd == UINT32_MAX){
        printf("Failed to open hello.txt\n");
        while(true);
    }
    char* buf = (char*)malloc(20);
    siv_read(fd,0,buf,20);
    printf("hello.txt: %s\n",buf);
    siv_close(fd);

    hive_header* hive = read_hive("hive.reg");
    if(hive == NULL){
        printf("Failed to read hive.reg\n");
        while(true);
    }
    printf("hive.reg's name: %s\n",hive->name);
    free_hive(hive);
 
    while(true);
}