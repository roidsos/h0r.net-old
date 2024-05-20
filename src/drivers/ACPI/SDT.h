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

_bool do_checksum(sdt_header *table_header);

#endif
