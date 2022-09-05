#include <filesystem/fat.h>
#include <util/printf.h>
#include <memory/Heap.h>
#include <util/logger.h>
void Loadfat32(ATA ata,fat_BS* fatbs,fat_extBS_32* BSext){
    //data stuff
    uint_32 fatstart = fatbs->reserved_sector_count;
    uint_32 fatsize  = BSext->table_size_32;
    uint_32 datastart= fatstart + (fatsize*fatbs->table_count);
    uint_32 rootstart= datastart+ (fatbs->sectors_per_cluster*(BSext->root_cluster - 2));


    DirectoryEntryFat32* dirents = malloc(16*sizeof(DirectoryEntryFat32));
    ata.Read28(rootstart,(uint_8*)dirents,16*sizeof(DirectoryEntryFat32));
    printf("files:\n");
    for (size_t i = 0; i < 16; i++)
    {
        if(dirents[i].name == 0x00)
            break;

        if((dirents[i].attributes & 0x0f) == 0x00)//skip non-existing files
            continue;
        char* name = malloc(8);
        char* ext = malloc(3);
        for (size_t j = 0; j < 8; j++)
            name[j] = dirents[i].name[j];
        for (size_t j = 0; j < 8; j++)
            ext[j] = dirents[i].ext[j];
        
        printf("file : %s.%s\n",name,ext);
        
    // kész a névkiírás,  rakj a lemere filokat hogy teszelhessünk okes mountolom 
    // csak ... . . . . .
    }
    
}

void LoadMBR(ATA ata){
    uint_8 *sectorbuffer = malloc(512);
    ata.Read28(0, sectorbuffer, 512);
    ata.Flush();
    fat_BS *fat_boot = (fat_BS *)sectorbuffer;
    fat_extBS_32 *fat_boot_ext = (fat_extBS_32 *)fat_boot->extended_section;



   // printf("oem: %s\n", fat_boot->oem_name); // idk what to do with this information that is in the struct
                                             // In this case we EXPECT the machine to have a pre formatted fat32 disk (:

    //uint_32 fat_size = (fat_boot->table_size_16 == 0)? fat_boot_ext->table_size_32 : fat_boot->table_size_16;
    //uint_32 root_dir_sectors = ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
    //uint_32 data_sectors = fat_boot->total_sectors_16 - (fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size) + root_dir_sectors);
    //uint_32 total_clusters = data_sectors / fat_boot->sectors_per_cluster;

    //if (fat_boot->bytes_per_sector == 0)
    //{
    //    LogERR("Error mounting device,Exfat is not supported\n");
    //}
    //else if (total_clusters < 4085)
    //{
    //   LogERR("Error mounting device,fat12 is not supported\n");
    //}
    //else if (total_clusters < 65525)
    //{
    //    LogERR("Error mounting device,fat16 is not supported\n");
    //}
    //else
    //{
    //    LogINFO("fat32 detected on device\n");
    Loadfat32(ata,fat_boot,fat_boot_ext);
    //}
}