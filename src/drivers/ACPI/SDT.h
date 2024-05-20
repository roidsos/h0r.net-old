#ifndef SDT_H
#define SDT_H

#include <libk/stdint.h>
#include <libk/stdbool.h>
#include <libk/stddef.h>
typedef struct {
  char signature[4];
  u32 length;
  u8 revision;
  u8 checksum;
  char OEM_ID[6];
  char OEM_table_ID[8];
  u32 OEM_revision;
  u32 creator_ID;
  u32 creator_revision;
} __attribute__ ((packed)) sdt_header;

typedef struct {
    sdt_header h;
    u32 SDTs[];
} RSDT;

typedef struct {
    sdt_header h;
    u64 SDTs[];
} XSDT;

typedef struct{
    char signature[8];
    u8 checksum;
    char OEM_ID[6];
    u8 revision;
    u32 RSDT_address;

    u32 length;
    u64 XSDT_address;
    u8 extended_checksum;
    u8 reserved[3];
} __attribute__ ((packed)) XSDP;

typedef struct {
    u8 address_space;
    u8 bit_width;
    u8 bit_offset;
    u8 access_size;
    u64 address;
} __attribute((packed)) generic_address_t;


typedef struct {
    sdt_header h;
    u32 firmware_ctrl;
    u32 dsdt;

    //ACPI 1.0 remainant
    u8 reserved;

    u8 preferred_power_management_profile;

    u16 SCI_interrupt;
    u32 SMI_command_port;

    u8 ACPI_enable;
    u8 ACPI_disable;

    u8 S4_BIOS_REQ;
    u8 PSTATE_control;

    u32 PM1a_event_block;
    u32 PM1b_event_block;
    u32 PM1a_control_block;
    u32 PM1b_control_block;
    u32 PM2_control_block;
    u32 PM_timer_block;

    u32 GPE0_block;
    u32 GPE1_block;

    u8 PM1_event_length;
    u8 PM1_control_length;
    u8 PM2_control_length;
    u8 PM_timer_length;

    u8 GPE0_length;
    u8 GPE1_length;
    u8 GPE1_base;

    u8 C_state_control;

    u16 worst_C2_latency;
    u16 worst_C3_latency;

    u16 flush_size;
    u16 flush_stride;

    u8 duty_offset;
    u8 duty_width;

    u8 day_alarm;
    u8 month_alarm;
    u8 century;

    //ACPI 2.0+
    u16 boot_architecture_flags;

    u8 reserved2;
    u32 flags;

    generic_address_t reset_reg;
    u8 reset_value;

    u8 Filler[3];

    //64 bit pointers only work on 64 bit systems, but h0r.net wont boot on 32 bit systems anyways
    u64 X_Firmware_Control;
    u64 X_Dsdt;

    generic_address_t X_PM1a_event_block;
    generic_address_t X_PM1b_event_block;
    generic_address_t X_PM1a_control_block;
    generic_address_t X_PM1b_control_block;
    generic_address_t X_PM2_control_block;
    generic_address_t X_PM_timer_block;
    generic_address_t X_GPE0_block;
    generic_address_t X_GPE1_block;
} __attribute((packed)) fadt_header;


_bool find_rsdt(void* RSDP_address);

_bool do_checksum(sdt_header *table_header);

sdt_header *find_SDT(char* signature);
sdt_header *find_nth_SDT(char *signature,usize index);

#endif
