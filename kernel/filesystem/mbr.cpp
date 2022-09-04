#include <filesystem/mbr.h>
#include <util/stdint.h>
#include <util/printf.h>
#include <memory/Heap.h>
#include <util/logger.h>
void LoadMBR(ATA ata)
{   
   uint_8* sectorbuffer = malloc(512);
   ata.Read28(0, sectorbuffer, 512);
   ata.Flush();
   fat_BS* fat_boot = (fat_BS_t*)sectorbuffer;

   fat_extBS_16* fat_boot_ext_16 = (fat_extBS_16*)fat_boot->extended_section; 

   fat_extBS_32* fat_boot_ext_32 = (fat_extBS_32*)fat_boot->extended_section; 
    
    if(sectorbuffer[510] == 0xAA && sectorbuffer[511] == 0x55 ) LogINFO("drive is bootable \n");

   printf("oem: %s\n", fat_boot->oem_name); // idk what to do with this information that is in the struct
   // In this case we EXPECT the machine to have a pre formatted fat32 disk (:



    

    uint_32 total_sectors = (fat_boot->total_sectors_16 == 0)? fat_boot->total_sectors_32 : fat_boot->total_sectors_16;
    uint_32 fat_size = (fat_boot->table_size_16 == 0)? fat_boot_ext_32->table_size_32 : fat_boot->table_size_16;
    uint_16 root_dir_sectors = ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
    uint_32 first_data_sector = fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size) + root_dir_sectors;
    uint_16 first_fat_sector = fat_boot->reserved_sector_count;
    uint_32 data_sectors = fat_boot->total_sectors_16 - (fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size) + root_dir_sectors);
    uint_32 total_clusters = data_sectors / fat_boot->sectors_per_cluster;
    uint_8  fat_type;

if (fat_boot->bytes_per_sector == 0) 
{
   fat_type = 0;//ExFat
   LogERR("Error mounting device,Exfat is not supported\n");
}
else if(total_clusters < 4085) 
{
   fat_type = 1;//fat12
   LogINFO("fat12 detected on device\n");
} 
else if(total_clusters < 65525) 
{
   fat_type = 2;//fat16
   LogINFO("fat16 detected on device\n");
} 
else
{
   fat_type = 3;//fat32
   LogINFO("fat32 detected on device\n");
}

//continue here
}