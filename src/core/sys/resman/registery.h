#ifndef __REGISTERY_H__
#define __REGISTERY_H__

#include <libk/stdtypes.h>

typedef struct {
    uint32_t magic;   // a magic value meant to verify whether this file is actually a hive file
    uint32_t num_keys; // the number of keys in the file
    uint8_t checksum; // a checksum that makes all bytes add up to 0x00
    char name[64];    // the name of the hive
} __attribute__((packed)) hive_header;

typedef struct {
    uint32_t magic;         // magic number
    uint32_t num_entries;   // the number of entries
    uint32_t num_subkeys;   // the number of subkeys
    char name[64];          // the name of the key
} __attribute__((packed)) key_header;

typedef struct {
    char name[64]; // The name of the entry
    uint8_t type;   // The type of the entry.
    uint8_t length; // The length of the entry not including the header.
} __attribute__((packed)) entry_header;

hive_header* read_hive(char* path);
void free_hive(hive_header* hive);

key_header* read_key(hive_header* hive, char* path);
entry_header* read_entry(key_header* key, char* name);

#endif 