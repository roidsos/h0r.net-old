#ifndef __SIV_H__
#define __SIV_H__

#include <types/stdtypes.h>

//Signatures of physical drive types
#define SIV_SIG_HARDDRIVE    "HARD"
#define SIV_SIG_FLOPPY_DRIVE "FLOP"
#define SIV_SIG_CD_DRIVE     "CDVD"
#define SIV_SIG_USB_DRIVE    "USBV"

//Signatures of virtual drive types
#define SIV_SIG_INITRD       "INRD"
#define SIV_SIG_STDIO        "STIO"
#define SIV_SIG_STDERR       "STER"
#define SIV_SIG_REGISTERY    "REGS"

#define SIV_INTENTS_READ     1 << 0
#define SIV_INTENTS_WRITE    1 << 1
#define SIV_INTENTS_EXEC     1 << 2

typedef struct{
    // Basic info
    uint16_t drive_id;
    char* name;
    char* full_path;
    uint64_t size;

    // Time stuffs
    uint64_t created;
    uint64_t last_modified;
    uint64_t last_accessed;

    // security stuffs
    uint64_t owner_id;
    uint64_t group_id;

    uint8_t owner_perms;
    uint8_t group_perms;
    uint8_t other_perms;
} file_t;

typedef struct {
    bool is_virtual;
    // File functions
    file_t (*get_props)(void* driver_specific_data,char* path);
    void   (*chmod    )(void* driver_specific_data,char* path,uint8_t perms,uint8_t who);
    void   (*chown    )(void* driver_specific_data,char* path,uint64_t uid,uint64_t gid);
    void   (*read     )(void* driver_specific_data,char* path,uint32_t offset,char* buffer,uint32_t size);
    void   (*write    )(void* driver_specific_data,char* path,uint32_t offset,char* buffer,uint32_t size);

    // FS functions
    void   (*mkfile   )(void* driver_specific_data,char* path,bool isdir,bool recursive);
    void   (*mklink   )(void* driver_specific_data,char* from,char* to);
    void   (*remove   )(void* driver_specific_data,char* path);
    void   (*copy     )(void* driver_specific_data,char* from,char* to);
    void   (*move     )(void* driver_specific_data,char* from,char* to);
} block_driver_t;

typedef struct {
    char sig[4];
    uint16_t driver_id;
    void* driver_specific_data;
} siv_drive_t;

void siv_init();
uint16_t siv_register_drive(siv_drive_t drive);
uint16_t siv_register_driver(block_driver_t driver);
uint32_t siv_open(uint32_t drive_id,char* path,uint8_t intents);
void siv_close(uint32_t file_desc);

void siv_read(uint32_t file_desc, uint32_t offset, char* buf, uint32_t size);
void siv_write(uint32_t file_desc, uint32_t offset, char* data, uint32_t size);

#endif