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

bool locate_rsdt();
sdt_header *find_thingy(char* signature);

#endif
