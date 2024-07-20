#include "AHCI.h"
#include "core/mm/mem.h"
#include "core/libk/string.h"
#include "core/mm/heap.h"
#include "core/utils/log.h"
#include <core/drivers/meta/PCI.h>
#include <core/libk/endian.h>

HBA_mem *abar;

void start_cmd(HBA_port *port)
{
	while (port->cmd & AHCI_HBA_CMD_CR);

	port->cmd |= AHCI_HBA_CMD_FRE;
	port->cmd |= AHCI_HBA_CMD_ST; 
}

void stop_cmd(HBA_port *port)
{
	port->cmd &= ~AHCI_HBA_CMD_ST;
	port->cmd &= ~AHCI_HBA_CMD_FRE;

	while(1)
	{
		if (port->cmd & AHCI_HBA_CMD_FR)
			continue;
		if (port->cmd & AHCI_HBA_CMD_CR)
			continue;
		break;
	}
 
}

u8 find_cmdslot(HBA_port *port)
{
	u32 slots = (port->sact | port->ci);
    u32 num_slots= (abar->cap & 0x0f00)>>8 ;
	for (u32 i=0; i < num_slots; i++)
	{
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
	log_error("Cannot find free command list entry\n");
	return 0xff;
}

static u32 check_type(HBA_port *port) {
    u32 ssts = port->ssts;

    u32 ipm = (ssts >> 8) & 0x0F;
    u32 det = ssts & 0x0F;

    if (det != AHCI_HBA_DET_PRESENT)
        return AHCI_DEV_TYPE_NULL;
    if (ipm != AHCI_HBA_IPM_ACTIVE)
        return AHCI_DEV_TYPE_NULL;

    switch (port->sig) {
    case AHCI_DEV_SIG_SATAPI:
        return AHCI_DEV_TYPE_SATAPI;
    case AHCI_DEV_SIG_SEMB:
        return AHCI_DEV_TYPE_SEMB;
    case AHCI_DEV_SIG_PM:
        return AHCI_DEV_TYPE_PM;
    default:
        return AHCI_DEV_TYPE_SATA;
    }
}

void port_rebase(HBA_port *port) {
    stop_cmd(port);

    //command list is 1024 bytes per SATA port
    u64 clb = (u64)VIRT_TO_PHYS(malloc(1024));
    memset(PHYS_TO_VIRT(clb), 0, 1024);
    port->clb = (u32)clb;
    port->clbu = (u32)(clb << 32);

    //FIS buffer is 256 bytes per SATA port
    u64 fb = (u64)VIRT_TO_PHYS(malloc(256));
    memset(PHYS_TO_VIRT(fb), 0, 256);
    port->fb = (u32)fb;
    port->fbu = (u32)(fb << 32);

    //Command table is 8K per port
    HBA_command_header *ct_entries = (HBA_command_header *)clb;
    for (int i = 0; i < 32; i++) {
        ct_entries[i].prdtl = 8;
        // Command table buffer is 256 bytes
        u64 ctba = (u64)VIRT_TO_PHYS(malloc(256));
        memset(PHYS_TO_VIRT(ctba), 0, 256);
        ct_entries[i].ctba = (u32)ctba;
        ct_entries[i].ctbau = (u32)(ctba << 32);
    }

    start_cmd(port);
}

_bool ahci_use_device(HBA_port *port,u64 start , u8 *buf, u32 count,_bool write) {
    port->is = 0xffff;

    u8 slot = 0;

    if((slot = find_cmdslot(port)) == 0xff) return false;
    u64 addr = (u64)PHYS_TO_VIRT(port->clb | ((u64)port->clbu << 32));

    HBA_command_header *ct_entries = (HBA_command_header *)addr;
    ct_entries[slot].cfl = sizeof(FIS_reg_h2d)/sizeof(u32);
    ct_entries[slot].w = write;
    ct_entries[slot].c = 1;
    ct_entries[slot].p = 1;
    ct_entries[slot].prdtl = (u16)((count-1)>>4) + 1;

    addr = (u64)PHYS_TO_VIRT(ct_entries[slot].ctba | ((u64)ct_entries[slot].ctbau << 32));
    HBA_command_table *ct = (HBA_command_table *)addr;
    for(int i = 0; i < ct_entries[slot].prdtl - 1; i++) {
        ct->prdt_entry[i].dba = (u32)((u64)buf & 0xffffffff);
        ct->prdt_entry[i].dbau = (u32)(((u64)buf << 32) & 0xffffffff);
        ct->prdt_entry[i].dbc = 8*1024-1;
        ct->prdt_entry[i].i = 0;

        buf += 8*1024; // advance by 16 sectors
        count -= 16;
    }

    // The last sector has to be different
    ct->prdt_entry[ct_entries[slot].prdtl-1].dba = (u32)((u64)buf & 0xffffffff);
    ct->prdt_entry[ct_entries[slot].prdtl-1].dbau = (u32)(((u64)buf << 32) & 0xffffffff);
    ct->prdt_entry[ct_entries[slot].prdtl-1].dbc = count << 9;
    ct->prdt_entry[ct_entries[slot].prdtl-1].i = 0;


    // Setup command
    FIS_reg_h2d *cmdfis = (FIS_reg_h2d *)&ct->cfis;

    cmdfis->fis_type = FIS_TYPE_REG_H2D;
    cmdfis->c = 1;
    cmdfis->command = write ? AHCI_CMD_WRITE_DMA_EX : AHCI_CMD_READ_DMA_EX;

    cmdfis->device = 1<<6; //LBA mode
    cmdfis->lba0 = start;
    cmdfis->lba1 = start >> 8;
    cmdfis->lba2 = start >> 16;
    cmdfis->lba3 = start >> 24;
    cmdfis->lba4 = start >> 32;
    cmdfis->lba5 = start >> 40;

    cmdfis->countl = count;
    cmdfis->counth = count >> 8;


    port->ci = 1;
    while((port->ci & (1 << slot)) == 0){
        if(port->is & AHCI_HBA_IS_TFES){
            log_error("AHCI: could not read from device\n");
            return false;// Task file error
        } 
    }
    // check again, doesn't hurt
    if(port->is & AHCI_HBA_IS_TFES){
            log_error("AHCI: could not read from device\n");
            return false;// Task file error
    }
    return true;
}
_bool ahci_init() {
    pci_multi_dev_t devs = pci_find_devices_by_class(0x1, 0x6);

    if (devs.count == 0) {
        log_error("AHCI: No devices found\n");
        return false;
    }
    // TODO: check for multiple AHCI controllers
    abar = (HBA_mem *)PHYS_TO_VIRT(pci_get_bar5(devs.addrs[0]).addr);

    uint32_t pi = abar->pi;
    int i = 0;
    while (i < AHCI_MAX_PORTS) {
        if (pi & (1 << i)) {
            int dt = check_type(&abar->ports[i]);
            if (dt == AHCI_DEV_TYPE_SATA) {
                log_trace("SATA drive found at port %d!\n", i);
                port_rebase(&abar->ports[i]);
            } else if (dt == AHCI_DEV_TYPE_SATAPI) {
                log_trace("SATAPI is unimplemented...\n", i);
            } else if (dt == AHCI_DEV_TYPE_SEMB) {
                log_trace("SMBus is unimplemented...\n", i);
            } else if (dt == AHCI_DEV_TYPE_PM) {
                log_trace("PM is unimplemented...\n", i);
            } else {
                log_trace("Что за устройство находится на порту %d?\n", i);
            }
        }
        i++;
    }

    return true;
}
