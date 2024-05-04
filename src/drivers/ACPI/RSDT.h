#ifndef RSDT_H
#define RSDT_H

#include "SDT.h"

typedef struct {
    sdt_header h;
    uint32_t SDTs[];
} RSDT;

typedef struct {
    sdt_header h;
    uint64_t SDTs[];
} XSDT;


typedef struct{
    char signature[8];
    uint8_t checksum;
    char OEM_ID[6];
    uint8_t revision;
    uint32_t RSDT_address;

    uint32_t length;
    uint64_t XSDT_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__ ((packed)) XSDP;

typedef struct {
    uint8_t address_space;
    uint8_t bit_width;
    uint8_t bit_offset;
    uint8_t access_size;
    uint64_t address;
} __attribute((packed)) generic_address_t;


typedef struct {
    sdt_header h;
    uint32_t firmware_ctrl;
    uint32_t dsdt;

    //ACPI 1.0 remainant
    uint8_t reserved;

    uint8_t preferred_power_management_profile;

    uint16_t SCI_interrupt;
    uint32_t SMI_command_port;

    uint8_t ACPI_enable;
    uint8_t ACPI_disable;

    uint8_t S4_BIOS_REQ;
    uint8_t PSTATE_control;

    uint32_t PM1a_event_block;
    uint32_t PM1b_event_block;
    uint32_t PM1a_control_block;
    uint32_t PM1b_control_block;
    uint32_t PM2_control_block;
    uint32_t PM_timer_block;

    uint32_t GPE0_block;
    uint32_t GPE1_block;

    uint8_t PM1_event_length;
    uint8_t PM1_control_length;
    uint8_t PM2_control_length;
    uint8_t PM_timer_length;

    uint8_t GPE0_length;
    uint8_t GPE1_length;
    uint8_t GPE1_base;

    uint8_t C_state_control;

    uint16_t worst_C2_latency;
    uint16_t worst_C3_latency;

    uint16_t flush_size;
    uint16_t flush_stride;

    uint8_t duty_offset;
    uint8_t duty_width;

    uint8_t day_alarm;
    uint8_t month_alarm;
    uint8_t century;

    //ACPI 2.0+
    uint16_t boot_architecture_flags;

    uint8_t reserved2;
    uint32_t flags;

    generic_address_t reset_reg;
    uint8_t reset_value;

    uint8_t Filler[3];

    //64 bit pointers only work on 64 bit systems, but h0r.net wont boot on 32 bit systems anyways
    uint64_t X_Firmware_Control;
    uint64_t X_Dsdt;

    generic_address_t X_PM1a_event_block;
    generic_address_t X_PM1b_event_block;
    generic_address_t X_PM1a_control_block;
    generic_address_t X_PM1b_control_block;
    generic_address_t X_PM2_control_block;
    generic_address_t X_PM_timer_block;
    generic_address_t X_GPE0_block;
    generic_address_t X_GPE1_block;
} __attribute((packed)) fadt_header;

bool locate_rsdt();
sdt_header *find_thingy(char* signature);
sdt_header *find_nth_thingy(char *signature,size_t index);

#endif
