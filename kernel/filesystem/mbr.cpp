#include <filesystem/mbr.h>
#include <util/stdint.h>
#include <util/printf.h>
void LoadMBR(ATA ata)
{
    fat_BS *fat_boot;
    ata.Read28(0, (uint_8 *)fat_boot, sizeof(fat_BS));//szerintem a read azya
    ata.Flush();
    printf("oem: %s", fat_boot->oem_name); // idk what to do with this information that is in the struct
    // In this case we EXPECT the machine to have a pre formatted fat32 disk (:
    

}