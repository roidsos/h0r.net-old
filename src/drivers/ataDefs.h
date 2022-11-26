
// code taken from https://github.com/ilobilo/kernel/blob/master/source/drivers/block/ata/ata.hpp and modified !!
#define    ATA_REGISTER_DATA           portbase + 0x00
#define    ATA_REGISTER_ERROR          portbase + 0x01
#define    ATA_REGISTER_FEATURES       portbase + 0x01
#define    ATA_REGISTER_SECTOR_COUNT   portbase + 0x02
#define    ATA_REGISTER_LBA_LOW        portbase + 0x03
#define    ATA_REGISTER_LBA_MID        portbase + 0x04
#define    ATA_REGISTER_LBA_HIGH       portbase + 0x05
#define    ATA_REGISTER_DRIVE_HEAD     portbase + 0x06
#define    ATA_REGISTER_STATUS         portbase + 0x07
#define    ATA_REGISTER_COMMAND        portbase + 0x07

#define    ATA_IDENT_DEVICETYPE    0
#define    ATA_IDENT_CYLINDERS     1
#define    ATA_IDENT_HEADS         3
#define    ATA_IDENT_SECTORS       6
#define    ATA_IDENT_SERIAL        10
#define    ATA_IDENT_MODEL         27
#define    ATA_IDENT_CAPABILITIES  49
#define    ATA_IDENT_FIELDVALID    53
#define    ATA_IDENT_MAX_LBA       160
#define    ATA_IDENT_COMMANDSETS   82
#define    ATA_IDENT_MAX_LBA_EXT   100

#define    ATA_DEV_BUSY  0x80
#define    ATA_DEV_DRDY  0x40
#define    ATA_DEV_DF    0x20
#define    ATA_DEV_DSC   0x10
#define    ATA_DEV_DRQ   0x08
#define    ATA_DEV_CORR  0x04
#define    ATA_DEV_IDX   0x02
#define    ATA_DEV_ERR   0x01

#define    ATA_CMD_READ_DMA_EX       0x25
#define    ATA_CMD_READ_EXT          0x24
#define    ATA_CMD_READ              0x20
#define    ATA_CMD_WRITE_DMA_EX      0x35
#define    ATA_CMD_WRITE_EXT         0x34
#define    ATA_CMD_WRITE             0x30
#define    ATA_CMD_IDENTIFY          0xEC
#define    ATAPI_CMD_PACKET          0xA0
#define    ATAPI_CMD_IDENTIFY        0xEC
#define    ATAPI_CMD_IDENTIFY_PACKET 0xA1
#define    ATAPI_CMD_READ            0xA8
#define    ATAPI_CMD_EJECT           0x1B
