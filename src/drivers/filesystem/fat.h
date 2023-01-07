    #include <drivers/mass-storage.h>
    #include <util/stdint.h>

    void LoadFAT(uint_8 partition);

        struct DirectoryEntryFat32
    {
        uint_8 name[8];
        uint_8 ext[3];
        uint_8 attributes;
        uint_8 reserved;
        uint_8 cTimeTenth;
        uint_16 cTime;
        uint_16 cDate;
        uint_16 aTime;
        uint_16 firstClusterHi;
        uint_16 wTime;
        uint_16 wDate;
        uint_16 firstClusterLow;
        uint_32 size;
    } __attribute__((packed));
    
    typedef struct fat_extBS_32
    {
        // extended fat32 stuff
        unsigned int table_size_32;
        unsigned short extended_flags;
        unsigned short fat_version;
        unsigned int root_cluster;
        unsigned short fat_info;
        unsigned short backup_BS_sector;
        unsigned char reserved_0[12];
        unsigned char drive_number;
        unsigned char reserved_1;
        unsigned char boot_signature;
        unsigned int volume_id;
        unsigned char volume_label[11];
        unsigned char fat_type_label[8];

    } __attribute__((packed)) fat_extBS_32_t;

    typedef struct fat_extBS_16
    {
        // extended fat12 and fat16 stuff
        unsigned char bios_drive_num;
        unsigned char reserved1;
        unsigned char boot_signature;
        unsigned int volume_id;
        unsigned char volume_label[11];
        unsigned char fat_type_label[8];

    } __attribute__((packed)) fat_extBS_16_t;

    typedef struct fat_BS
    {
        unsigned char bootjmp[3];
        unsigned char oem_name[8];
        unsigned short bytes_per_sector;
        unsigned char sectors_per_cluster;
        unsigned short reserved_sector_count;
        unsigned char table_count;
        unsigned short root_entry_count;
        unsigned short total_sectors_16;
        unsigned char media_type;
        unsigned short table_size_16;
        unsigned short sectors_per_track;
        unsigned short head_side_count;
        unsigned int hidden_sector_count;
        unsigned int total_sectors_32;

        // this will be cast to it's specific type once the driver actually knows what type of FAT this is.
        unsigned char extended_section[54];

    } __attribute__((packed)) fat_BS_t;