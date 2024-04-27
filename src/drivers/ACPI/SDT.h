#ifndef SDT_H
#define SDT_H

#include <types/stdtypes.h>

typedef struct {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char OEM_ID[6];
  char OEM_table_ID[8];
  uint32_t OEM_revision;
  uint32_t creator_ID;
  uint32_t creator_revision;
} __attribute__ ((packed)) sdt_header;

bool do_checksum(sdt_header *table_header);

#endif
