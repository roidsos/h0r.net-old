#ifndef __SIV_H__
#define __SIV_H__

#include <cstdint>
#include <types/stdtypes.h>

//Spooky scary skeleton lmao

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

typedef struct {
    char sig[4];
    void* driver_specific_data;
} siv_drive_t;

void siv_init();
void siv_register_drive(siv_drive_t drive);
uint32_t siv_open(uint32_t frive_id,char* path,uint8_t intents);
void siv_close(uint32_t file_desc);

char *siv_read(uint32_t file_desc, uint32_t offset, uint32_t size);
void siv_write(uint32_t file_desc, uint32_t offset, char* data, uint32_t size);

#endif