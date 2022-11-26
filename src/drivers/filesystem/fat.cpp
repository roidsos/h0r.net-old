#include <drivers/filesystem/fat.h>
#include <util/printf.h>
#include <drivers/memory/Heap.h>
#include <drivers/memory/memory.h>
#include <util/logger.h>
void Loadfat32(fat_BS* fatbs,fat_extBS_32* BSext,uint_8 partition){
    //theese
    uint_32 fatstart = partition + fatbs->reserved_sector_count;
    uint_32 fatsize  = BSext->table_size_32;
    uint_32 datastart= fatstart + (fatsize*fatbs->table_count);
    uint_32 rootstart= datastart+ (fatbs->sectors_per_cluster*(BSext->root_cluster - 2));


    // ye ah shit I forgor I need to link disk first hold on
    
    printf("fatstart: %d\n",fatstart);
    printf("fatsize: %d\n",fatsize);
    printf("datastart: %d\n",datastart);
    printf("fatsize: %d\n",rootstart);

    printf("rootstart : %i \n",rootstart);
    uint_8* sectorbuffer = calloc(16*sizeof(DirectoryEntryFat32));

    //TODO: replace the ATA reads in this file with the new ata driver reads when its done

    ATA::Read28(0,rootstart, sectorbuffer, 16*sizeof(DirectoryEntryFat32));
    DirectoryEntryFat32* dirents = (DirectoryEntryFat32*)sectorbuffer;

    //for(int i = 0; i <= 0x01FF; i++) // testing the data
    //{
    //    printf("0x%x ",sectorbuffer[i]);
    //}
    //printf("\n");
    //return;
    printf("files:\n");


    for (size_t i = 0; i < 16; i++)
    {
        if(dirents[i].name[0] == 0x00){
            //printf("nothing :/\n");
            continue;
            }
        if((dirents[i].attributes & 0x0F) == 0x0F){//skip directories
            continue;
        }
        
        char* name = malloc(8);
        char* ext = malloc(3);
        for (size_t j = 0; j < 8; j++)
            name[j] = dirents[i].name[j];
        for (size_t j = 0; j < 3; j++)
            ext[j] = dirents[i].ext[j];
        
        printf("file : %s.%s\n",name,ext);
        
    }

    
    
}

void LoadFAT(uint_8 partition){
    uint_8 *sectorbuffer = calloc(512);
    ATA::Read28(0,partition, sectorbuffer, 512);
    fat_BS *fat_boot = (fat_BS *)sectorbuffer;
    fat_extBS_32* fat_boot_ext = (fat_extBS_32 *)fat_boot->extended_section;

    Loadfat32(fat_boot,fat_boot_ext,partition);

}