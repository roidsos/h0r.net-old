#ifndef __AHCI_H__
#define __AHCI_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>

typedef volatile struct {
	u32 clb;		// 0x00, command list base address, 1K-byte aligned
	u32 clbu;		// 0x04, command list base address upper 32 bits
	u32 fb;		    // 0x08, FIS base address, 256-byte aligned
	u32 fbu;		// 0x0C, FIS base address upper 32 bits
	u32 is;		    // 0x10, interrupt status
	u32 ie;		    // 0x14, interrupt enable
	u32 cmd;		// 0x18, command and status
	u32 rsv0;		// 0x1C, Reserved
	u32 tfd;		// 0x20, task file data
	u32 sig;		// 0x24, signature
	u32 ssts;		// 0x28, SATA status (SCR0:SStatus)
	u32 sctl;		// 0x2C, SATA control (SCR2:SControl)
	u32 serr;		// 0x30, SATA error (SCR1:SError)
	u32 sact;		// 0x34, SATA active (SCR3:SActive)
	u32 ci;		    // 0x38, command issue
	u32 sntf;		// 0x3C, SATA notification (SCR4:SNotification)
	u32 fbs;		// 0x40, FIS-based switch control
	u32 rsv1[11];	// 0x44 ~ 0x6F, Reserved
	u32 vendor[4];	// 0x70 ~ 0x7F, vendor specific
} HBA_port_t;

typedef volatile struct {
	// 0x00 - 0x2B, Generic Host Control
	u32 cap;		// 0x00, Host capability
	u32 ghc;		// 0x04, Global host control
	u32 is;		    // 0x08, Interrupt status
	u32 pi;		    // 0x0C, Port implemented
	u32 vs;		    // 0x10, Version
	u32 ccc_ctl;	// 0x14, Command completion coalescing control
	u32 ccc_pts;	// 0x18, Command completion coalescing ports
	u32 em_loc;		// 0x1C, Enclosure management location
	u32 em_ctl;		// 0x20, Enclosure management control
	u32 cap2;		// 0x24, Host capabilities extended
	u32 bohc;		// 0x28, BIOS/OS handoff control and status
 
	// 0x2C - 0x9F, Reserved
	u8  rsv[0xA0-0x2C];
 
	// 0xA0 - 0xFF, Vendor specific registers
	u8  vendor[0x100-0xA0];
 
	// 0x100 - 0x10FF, Port control registers
	HBA_port_t	ports[1];	// 1 ~ 32
} HBA_mem_t;
_bool ahci_init();

#endif // __AHCI_H__