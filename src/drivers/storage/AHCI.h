#ifndef __AHCI_H__
#define __AHCI_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>

//all structs and values are from https://github.com/rajesh5310/SBUnix/blob/master/sys/ahci.c and https://osdev.org/AHCI 

#define AHCI_MAX_SLOTS 32

#define	AHCI_SIG_SATA	 0x00000101
#define	AHCI_SIG_SATAPI	 0xEB140101
#define	AHCI_SIG_SEMB	 0xC33C0101
#define	AHCI_SIG_PM		 0x96690101

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define AHCI_HBA_PORT_IPM_ACTIVE 1
#define AHCI_HBA_PORT_DET_PRESENT 3

#define	AHCI_BASE	0x400000	// 4M
 
#define AHCI_HBA_PxCMD_ST    0x0001
#define AHCI_HBA_PxCMD_FRE   0x0010
#define AHCI_HBA_PxCMD_FR    0x4000
#define AHCI_HBA_PxCMD_CR    0x8000

#define AHCI_DEV_BUSY 0x80
#define AHCI_DEV_DRQ 0x08

enum FIS_type
{
	FIS_TYPE_REG_H2D	= 0x27,
	FIS_TYPE_REG_D2H	= 0x34,
	FIS_TYPE_DMA_ACT	= 0x39,
	FIS_TYPE_DMA_SETUP	= 0x41,
	FIS_TYPE_DATA		= 0x46,
	FIS_TYPE_BIST		= 0x58,
	FIS_TYPE_PIO_SETUP	= 0x5F,
	FIS_TYPE_DEV_BITS	= 0xA1,
};

typedef struct
{
	u8  fis_type;
	u8  pmport:4;
	u8  rsv0:3;
	u8  c:1;
	u8  command;
	u8  featurel;
	u8  lba0;
	u8  lba1;
	u8  lba2;
	u8  device;
	u8  lba3;
	u8  lba4;
	u8  lba5;
	u8  featureh;
	u8  countl;
	u8  counth;
	u8  icc;
	u8  control;
	u8  rsv1[4];
} FIS_reg_h2d;

typedef struct
{
	u8  fis_type;
	u8  pmport:4;
	u8  rsv0:2;
	u8  i:1;
	u8  rsv1:1;
	u8  status;
	u8  error;
	u8  lba2;
	u8  device;
	u8  lba3;
	u8  lba4;
	u8  lba5;
	u8  rsv2;
	u8  countl;
	u8  counth;
	u8  rsv3[2];
	u8  rsv4[4];
} FIS_reg_d2h;

typedef struct 
{
	u8  fis_type;
	u8  pmport:4;
	u8  rsv0:4;
	u8  rsv1[2];
	u32 data[1];
} FIS_data;

typedef struct
{
	u8  fis_type;
	u8  pmport:4;
	u8  rsv0:1;
	u8  d:1;
	u8  i:1;
	u8  rsv1:1;
	u8  status;
	u8  error;
	u8  lba0;
	u8  lba1;
	u8  lba4;
	u8  lba5;
	u8  rsv2;
	u8  countl;
	u8  counth;
	u8  rsv3;
	u8  e_status;
	u16 tc;
	u8  rsv4[2];
} FIS_PIO_setup;

typedef struct 
{
	u8  fis_type;
	u8  pmport:4;
	u8  rsv0:1;
	u8  d:1;
	u8  i:1;
	u8  a:1;
    u8  rsved[2];
    u64 DMAbufferID;
    u32 rsvd;
    u32 DMAbufOffset;
    u32 TransferCount;
    u32 resvd;
 
} FIS_DMA_setup;

typedef volatile struct tagHBA_FIS
{
	FIS_DMA_setup	dsfis;
	u8              pad0[4];
	FIS_PIO_setup	psfis;
	u8              pad1[12];
	FIS_reg_d2h	    rfis;
	u8				pad2[4];
	u16				sdbfis;
	u8				ufis[64];
	u8				rsv[0x100-0xA0];
} HBA_FIS;

typedef struct
{
	u32 dba;
	u32 dbau;
	u32 rsv0;
	u32 dbc:22;
	u32 rsv1:9;	
	u32 i:1;	
} HBA_PRDT_entry;

typedef struct
{
	u8  cfis[64];
	u8  acmd[16];
	u8  rsv[48];
	HBA_PRDT_entry	prdt_entry[1];
} HBA_command_table;
 
typedef volatile struct {
	u32 clb;
	u32 clbu;
	u32 fb;
	u32 fbu;
	u32 is;
	u32 ie;
	u32 cmd;
	u32 rsv0;
	u32 tfd;
	u32 sig;
	u32 ssts;
	u32 sctl;
	u32 serr;
	u32 sact;
	u32 ci;
	u32 sntf;
	u32 fbs; 
	u32 rsv1[11];
	u32 vendor[4];
} HBA_port_t;

typedef volatile struct {
	u32 cap;
	u32 ghc;
	u32 is;
	u32 pi;
	u32 vs;
	u32 ccc_ctl;
	u32 ccc_pts;
	u32 em_loc;
	u32 em_ctl;
	u32 cap2;
	u32 bohc;
	u8  rsv[0xA0-0x2C];
	u8  vendor[0x100-0xA0];
	HBA_port_t	ports[1];
} HBA_mem_t;

_bool ahci_init();

#endif // __AHCI_H__